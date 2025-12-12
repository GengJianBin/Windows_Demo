// Test file for N-API adapter
const adapter = require('../src/index.js');

console.log('Testing N-API Adapter...\n');

// Test helloWorld
try {
    const result = adapter.helloWorld();
    console.log('✓ helloWorld():', result);
} catch (error) {
    console.error('✗ helloWorld() failed:', error.message);
}

// Test addNumbers
try {
    const result = adapter.addNumbers(10, 20);
    console.log('✓ addNumbers(10, 20):', result);
} catch (error) {
    console.error('✗ addNumbers() failed:', error.message);
}

// Test processString
try {
    const result = adapter.processString('Hello N-API');
    console.log('✓ processString("Hello N-API"):', result);
} catch (error) {
    console.error('✗ processString() failed:', error.message);
}

// Test createDataObject
try {
    const result = adapter.createDataObject();
    console.log('✓ createDataObject():', JSON.stringify(result, null, 2));
} catch (error) {
    console.error('✗ createDataObject() failed:', error.message);
}

// Test processArray
try {
    const result = adapter.processArray([1, 2, 3, 4, 5]);
    console.log('✓ processArray([1, 2, 3, 4, 5]):', result);
} catch (error) {
    console.error('✗ processArray() failed:', error.message);
}

console.log('\nAll tests completed!');




