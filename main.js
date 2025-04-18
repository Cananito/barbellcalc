let resultLabel;
let wasmObj;

// TODO: Delete if never used.
function stringFromCCharPointer(cCharPointer, length) {
  const wasmMemoryBuffer = wasmObj.instance.exports.memory.buffer;
  const stringBuffer = new Uint8Array(wasmMemoryBuffer, cCharPointer, length);
  let string = new TextDecoder().decode(stringBuffer);
  return string;
}

function stringFromNullTerminatedCCharPointer(cCharPointer) {
  const wasmMemoryBuffer = wasmObj.instance.exports.memory.buffer;
  let stringBuffer = new Uint8Array(wasmMemoryBuffer, cCharPointer);
  for (let i = 0; i < stringBuffer.byteLength; i++) {
    if (stringBuffer[i] === 0) {
      stringBuffer = stringBuffer.subarray(0, i);
      break;
    }
  }
  const string = new TextDecoder().decode(stringBuffer);
  return string;
}

function writeStringToWASMMemoryBuffer(string, destinationBufferPointer) {
  const stringBuffer = new TextEncoder().encode(string + "\0");
  const length = stringBuffer.length;
  const wasmMemoryBuffer = wasmObj.instance.exports.memory.buffer;
  const destinationBuffer = new Uint8Array(wasmMemoryBuffer,
                                           destinationBufferPointer,
                                           length);
  destinationBuffer.set(stringBuffer);
}

function cStringToInt(cStringCCharPointer) {
  const string = stringFromNullTerminatedCCharPointer(cStringCCharPointer);
  return parseInt(string);
}

function intToCString(i, destinationBufferPointer) {
  const string = i.toString();
  writeStringToWASMMemoryBuffer(string, destinationBufferPointer);
}

function initializeUI() {
  const body = document.body;

  resultLabel = document.createElement("span");
  resultLabel.innerHTML = "Loading...";
  body.appendChild(resultLabel);
}

function main() {
  initializeUI();

  const importObject = {
    env: {
      "string_to_int": cStringToInt,
      "int_to_string": intToCString,
    },
  };
  const wasmPromise = WebAssembly.instantiateStreaming(fetch("calc.wasm"),
                                                       importObject);
  wasmPromise.then(function (obj) {
    wasmObj = obj;
    const exports = obj.instance.exports;

    const platesStringBufferPointer = exports.js_plates_string_buffer;
    writeStringToWASMMemoryBuffer("45", platesStringBufferPointer);
    const resultPointer =
        exports.calc_weight_from_plates(platesStringBufferPointer);
    const result = stringFromNullTerminatedCCharPointer(resultPointer);
    resultLabel.innerHTML = "Result: " + result;
  });
};
document.addEventListener("DOMContentLoaded", main);
