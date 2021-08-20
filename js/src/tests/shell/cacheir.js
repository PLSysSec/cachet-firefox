// |reftest| skip-if(!xulRuntime.shell)

'use strict';

assertEq(CacheKind.GetProp.name, 'GetProp');
assertEq(JSON.stringify(CacheKind.GetProp.inputs), '{"value":"Val"}');
assertEq(CacheKind.GetProp.output, 'Val');
assertEq(CacheKind.GetProp.toString(), '[object CacheKind.GetProp]');

const ionICs = IonIC.extractAll((foo, bar, baz) => { foo.bar; foo[bar] = baz; });
assertEq(ionICs.length, 2);

const getPropIonIC = ionICs[0];
assertEq(getPropIonIC instanceof IC, true);
assertEq(getPropIonIC instanceof IonIC, true);
assertEq(getPropIonIC.kind, CacheKind.GetProp);

const getPropStub = getPropIonIC.generateStub({ bar: 42 });
assertEq(getPropStub instanceof ICStub, true);
assertEq(getPropStub.kind, getPropIonIC.kind);
assertEq(getPropStub.toString().startsWith('GetProp IonIC\n'), true);
assertEq(getPropStub.toString().indexOf('LoadFixedSlotResult') >= 0, true);

const megamorphicGetPropStub = getPropIonIC.generateMegamorphicStub({ bar: 42 });
assertEq(megamorphicGetPropStub instanceof ICStub, true);
assertEq(megamorphicGetPropStub.kind, getPropIonIC.kind);
assertEq(megamorphicGetPropStub.toString().startsWith('GetProp IonIC\n'), true);
assertEq(megamorphicGetPropStub.toString().indexOf('MegamorphicLoadSlotResult') >= 0, true);

assertEq(CacheKind.SetElem.name, 'SetElem');
assertEq(JSON.stringify(CacheKind.SetElem.inputs), '{"object":"Obj","id":"Val","rhs":"Val"}');
assertEq(CacheKind.SetElem.output, null);
assertEq(CacheKind.SetElem.toString(), '[object CacheKind.SetElem]');

const setElemIonIC = ionICs[1];
assertEq(setElemIonIC instanceof IC, true);
assertEq(setElemIonIC instanceof IonIC, true);
assertEq(setElemIonIC.kind, CacheKind.SetElem);

const setElemStub = setElemIonIC.generateStub({}, 'bar', 42);
assertEq(setElemStub instanceof ICStub, true);
assertEq(setElemStub.kind, setElemIonIC.kind);
assertEq(setElemStub.toString().startsWith('SetElem IonIC\n'), true);
assertEq(setElemStub.toString().indexOf('AddAndStoreFixedSlot') >= 0, true);

const megamorphicSetElemStub = setElemIonIC.generateMegamorphicStub({}, 'bar', 42);
assertEq(megamorphicSetElemStub instanceof ICStub, true);
assertEq(megamorphicSetElemStub.kind, setElemIonIC.kind);
assertEq(megamorphicSetElemStub.toString().startsWith('SetElem IonIC\n'), true);
assertEq(megamorphicSetElemStub.toString().indexOf('MegamorphicSetElement') >= 0, true);

reportCompare(true, true);
