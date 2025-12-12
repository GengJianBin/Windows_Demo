const addon = require('../build/Release/napi_adapter.node');

module.exports = {
    helloWorld: addon.helloWorld,
    addNumbers: addon.addNumbers,
    processString: addon.processString,
    createDataObject: addon.createDataObject,
    processArray: addon.processArray
};

