/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

var CaptivePortalWatcher = {
  // This is the value used to identify the captive portal notification.
  PORTAL_NOTIFICATION_VALUE: "captive-portal-detected",

  // This holds a weak reference to the captive portal tab so that we
  // don't leak it if the user closes it.
  _captivePortalTab: null,

  /**
   * If a portal is detected when we don't have focus, we first wait for focus
   * and then add the tab if, after a recheck, the portal is still active. This
   * is set to true while we wait so that in the unlikely event that we receive
   * another notification while waiting, we don't do things twice.
   */
  _delayedCaptivePortalDetectedInProgress: false,

  // In the situation above, this is set to true while we wait for the recheck.
  // This flag exists so that tests can appropriately simulate a recheck.
  _waitingForRecheck: false,

  // This holds a weak reference to the captive portal tab so we can close the tab
  // after successful login if we're redirected to the canonicalURL.
  _previousCaptivePortalTab: null,

  // We will only show the VPN promo if we're pretty sure that the captive
  // portal "Login" button has been pressed relatively recently
  // before the captive-portal-login-success notification was received so that
  // we can have reasonable degree of confidence that the user has some mental
  // context why when we might be showing them the promo.
  _loginButtonPressedTimeStamp: 0, // 0 is magic value meaning "not set"

  // Here's where we define "recently" for the button above:
  //
  // On the one hand, if for some reason the login doesn't happen (eg user
  // decides airplane wifi charge is too high), the next time they try to
  // log into either this or another captive portal, we could end up showing
  // the VPN promo when it's been a long time since they saw the UI, and
  // they've lost mental context.
  _LOGIN_BUTTON_PRESSED_TIMEOUT: 60 * 1000 * 1000, // 60 mins

  get _captivePortalNotification() {
    return gNotificationBox.getNotificationWithValue(
      this.PORTAL_NOTIFICATION_VALUE
    );
  },

  get canonicalURL() {
    return Services.prefs.getCharPref("captivedetect.canonicalURL");
  },

  get _browserBundle() {
    delete this._browserBundle;
    return (this._browserBundle = Services.strings.createBundle(
      "chrome://browser/locale/browser.properties"
    ));
  },

  init() {
    Services.obs.addObserver(this, "ensure-captive-portal-tab");
    Services.obs.addObserver(this, "captive-portal-login");
    Services.obs.addObserver(this, "captive-portal-login-abort");
    Services.obs.addObserver(this, "captive-portal-login-success");
    Services.obs.addObserver(this, "captive-portal-login-button-pressed");

    this._cps = Cc["@mozilla.org/network/captive-portal-service;1"].getService(
      Ci.nsICaptivePortalService
    );

    if (this._cps.state == this._cps.LOCKED_PORTAL) {
      // A captive portal has already been detected.
      this._captivePortalDetected();

      // Automatically open a captive portal tab if there's no other browser window.
      if (BrowserWindowTracker.windowCount == 1) {
        this.ensureCaptivePortalTab();
      }
    } else if (this._cps.state == this._cps.UNKNOWN) {
      // We trigger a portal check after delayed startup to avoid doing a network
      // request before first paint.
      this._delayedRecheckPending = true;
    }

    // This constant is chosen to be large enough for a portal recheck to complete,
    // and small enough that the delay in opening a tab isn't too noticeable.
    // Please see comments for _delayedCaptivePortalDetected for more details.
    XPCOMUtils.defineLazyPreferenceGetter(
      this,
      "PORTAL_RECHECK_DELAY_MS",
      "captivedetect.portalRecheckDelayMS",
      500
    );
  },

  uninit() {
    Services.obs.removeObserver(this, "ensure-captive-portal-tab");
    Services.obs.removeObserver(this, "captive-portal-login");
    Services.obs.removeObserver(this, "captive-portal-login-abort");
    Services.obs.removeObserver(this, "captive-portal-login-success");
    Services.obs.removeObserver(this, "captive-portal-login-button-pressed");

    this._cancelDelayedCaptivePortal();
  },

  delayedStartup() {
    if (this._delayedRecheckPending) {
      delete this._delayedRecheckPending;
      this._cps.recheckCaptivePortal();
    }
  },

  observe(aSubject, aTopic, aData) {
    switch (aTopic) {
      case "ensure-captive-portal-tab":
        this.ensureCaptivePortalTab();
        break;
      case "captive-portal-login":
        this._captivePortalDetected();
        break;
      case "captive-portal-login-abort":
        this._captivePortalGone(false);
        break;
      case "captive-portal-login-button-pressed":
        this._loginButtonPressedTimeStamp = Cu.now();
        break;
      case "captive-portal-login-success":
        this._captivePortalGone(true);

        if (
          this._loginButtonPressedTimeStamp &&
          Cu.now() - this._loginButtonPressedTimeStamp <
            this._LOGIN_BUTTON_PRESSED_TIMEOUT
        ) {
          Services.obs.notifyObservers(
            null,
            "captive-portal-login-success-after-button-pressed"
          );
          this._loginButtonPressedTimeStamp = 0;
        }
        break;
      case "delayed-captive-portal-handled":
        this._cancelDelayedCaptivePortal();
        break;
    }
  },

  onLocationChange(browser) {
    if (!this._previousCaptivePortalTab) {
      return;
    }

    let tab = this._previousCaptivePortalTab.get();
    if (!tab || !tab.linkedBrowser) {
      return;
    }

    if (browser != tab.linkedBrowser) {
      return;
    }

    // There is a race between the release of captive portal i.e.
    // the time when success/abort events are fired and the time when
    // the captive portal tab redirects to the canonicalURL. We check for
    // both conditions to be true and also check that we haven't already removed
    // the captive portal tab in the success/abort event handlers before we remove
    // it in the callback below. A tick is added to avoid removing the tab before
    // onLocationChange handlers across browser code are executed.
    Services.tm.dispatchToMainThread(() => {
      if (!this._previousCaptivePortalTab) {
        return;
      }

      tab = this._previousCaptivePortalTab.get();
      let canonicalURI = Services.io.newURI(this.canonicalURL);
      if (
        tab &&
        tab.linkedBrowser.currentURI.equalsExceptRef(canonicalURI) &&
        (this._cps.state == this._cps.UNLOCKED_PORTAL ||
          this._cps.state == this._cps.UNKNOWN)
      ) {
        gBrowser.removeTab(tab);
      }
    });
  },

  _captivePortalDetected() {
    if (this._delayedCaptivePortalDetectedInProgress) {
      return;
    }

    let win = BrowserWindowTracker.getTopWindow();

    // Used by tests: ignore the main test window in order to enable testing of
    // the case where we have no open windows.
    if (win.document.documentElement.getAttribute("ignorecaptiveportal")) {
      win = null;
    }

    // If no browser window has focus, open and show the tab when we regain focus.
    // This is so that if a different application was focused, when the user
    // (re-)focuses a browser window, we open the tab immediately in that window
    // so they can log in before continuing to browse.
    if (win != Services.focus.activeWindow) {
      this._delayedCaptivePortalDetectedInProgress = true;
      window.addEventListener("activate", this, { once: true });
      Services.obs.addObserver(this, "delayed-captive-portal-handled");
    }

    this._showNotification();
    Services.telemetry.recordEvent(
      "networking.captive_portal",
      "login_infobar_shown",
      "infobar"
    );
  },

  /**
   * Called after we regain focus if we detect a portal while a browser window
   * doesn't have focus. Triggers a portal recheck to reaffirm state, and adds
   * the tab if needed after a short delay to allow the recheck to complete.
   */
  _delayedCaptivePortalDetected() {
    if (!this._delayedCaptivePortalDetectedInProgress) {
      return;
    }

    // Used by tests: ignore the main test window in order to enable testing of
    // the case where we have no open windows.
    if (window.document.documentElement.getAttribute("ignorecaptiveportal")) {
      return;
    }

    Services.obs.notifyObservers(null, "delayed-captive-portal-handled");

    // Trigger a portal recheck. The user may have logged into the portal via
    // another client, or changed networks.
    this._cps.recheckCaptivePortal();
    this._waitingForRecheck = true;
    let requestTime = Date.now();

    let observer = () => {
      let time = Date.now() - requestTime;
      Services.obs.removeObserver(observer, "captive-portal-check-complete");
      this._waitingForRecheck = false;
      if (this._cps.state != this._cps.LOCKED_PORTAL) {
        // We're free of the portal!
        return;
      }

      if (time <= this.PORTAL_RECHECK_DELAY_MS) {
        // The amount of time elapsed since we requested a recheck (i.e. since
        // the browser window was focused) was small enough that we can add and
        // focus a tab with the login page with no noticeable delay.
        this.ensureCaptivePortalTab();
      }
    };
    Services.obs.addObserver(observer, "captive-portal-check-complete");
  },

  _captivePortalGone(aSuccess) {
    this._cancelDelayedCaptivePortal();
    this._removeNotification();

    if (!this._captivePortalTab) {
      return;
    }

    let tab = this._captivePortalTab.get();
    let canonicalURI = Services.io.newURI(this.canonicalURL);
    if (
      tab &&
      tab.linkedBrowser &&
      tab.linkedBrowser.currentURI.equalsExceptRef(canonicalURI)
    ) {
      this._previousCaptivePortalTab = null;
      gBrowser.removeTab(tab);
    }
    this._captivePortalTab = null;

    if (aSuccess) {
      Services.telemetry.recordEvent(
        "networking.captive_portal",
        "login_successful",
        "detector"
      );
    }
  },

  _cancelDelayedCaptivePortal() {
    if (this._delayedCaptivePortalDetectedInProgress) {
      this._delayedCaptivePortalDetectedInProgress = false;
      Services.obs.removeObserver(this, "delayed-captive-portal-handled");
      window.removeEventListener("activate", this);
    }
  },

  handleEvent(aEvent) {
    switch (aEvent.type) {
      case "activate":
        this._delayedCaptivePortalDetected();
        break;
      case "TabSelect":
        if (!this._captivePortalTab || !this._captivePortalNotification) {
          break;
        }

        let tab = this._captivePortalTab.get();
        let n = this._captivePortalNotification;
        if (!tab || !n) {
          break;
        }

        let doc = tab.ownerDocument;
        let button = n.buttonContainer.querySelector(
          "button.notification-button"
        );
        if (doc.defaultView.gBrowser.selectedTab == tab) {
          button.style.visibility = "hidden";
        } else {
          button.style.visibility = "visible";
        }
        break;
    }
  },

  _showNotification() {
    if (this._captivePortalNotification) {
      return;
    }

    let buttons = [
      {
        label: this._browserBundle.GetStringFromName(
          "captivePortal.showLoginPage2"
        ),
        callback: () => {
          this.ensureCaptivePortalTab();

          Services.obs.notifyObservers(
            null,
            "captive-portal-login-button-pressed"
          );

          Services.telemetry.recordEvent(
            "networking.captive_portal",
            "login_button_pressed",
            "login_button"
          );

          // Returning true prevents the notification from closing.
          return true;
        },
      },
    ];

    let message = this._browserBundle.GetStringFromName(
      "captivePortal.infoMessage3"
    );

    let closeHandler = aEventName => {
      if (aEventName != "removed") {
        return;
      }
      gBrowser.tabContainer.removeEventListener("TabSelect", this);
    };

    gNotificationBox.appendNotification(
      message,
      this.PORTAL_NOTIFICATION_VALUE,
      "",
      gNotificationBox.PRIORITY_INFO_MEDIUM,
      buttons,
      closeHandler
    );

    gBrowser.tabContainer.addEventListener("TabSelect", this);
  },

  _removeNotification() {
    let n = this._captivePortalNotification;
    if (!n || !n.parentNode) {
      return;
    }
    n.close();
  },

  ensureCaptivePortalTab() {
    let tab;
    if (this._captivePortalTab) {
      tab = this._captivePortalTab.get();
    }

    // If the tab is gone or going, we need to open a new one.
    if (!tab || tab.closing || !tab.parentNode) {
      tab = gBrowser.addWebTab(this.canonicalURL, {
        ownerTab: gBrowser.selectedTab,
        triggeringPrincipal: Services.scriptSecurityManager.createNullPrincipal(
          {
            userContextId: gBrowser.contentPrincipal.userContextId,
          }
        ),
        disableTRR: true,
      });
      this._captivePortalTab = Cu.getWeakReference(tab);
      this._previousCaptivePortalTab = Cu.getWeakReference(tab);
    }

    gBrowser.selectedTab = tab;
  },
};
