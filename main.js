let inputTextField;
let resultLabel;
let wasmObj;

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

function inputTextFieldHandler(event) {
  const exports = wasmObj.instance.exports;
  const platesStringBufferPointer = exports.js_plates_string_buffer;
  writeStringToWASMMemoryBuffer(inputTextField.value, platesStringBufferPointer);
  const resultPointer =
      exports.calc_weight_from_plates(platesStringBufferPointer);
  const result = stringFromNullTerminatedCCharPointer(resultPointer);
  resultLabel.innerHTML = "Result: " + result;
}

function initializeUI() {
  const body = document.body;

  inputTextField = document.createElement("input");
  inputTextField.type = "text";
  inputTextField.oninput = inputTextFieldHandler;
  body.appendChild(inputTextField);

  body.appendChild(document.createElement("br"));

  resultLabel = document.createElement("span");
  body.appendChild(resultLabel);
}

function main() {
  initializeUI();

  const wasmPromise = WebAssembly.instantiateStreaming(fetch("calc.wasm"), {});
  wasmPromise.then(function (obj) {
    wasmObj = obj;
    inputTextFieldHandler(null);
  });
};
document.addEventListener("DOMContentLoaded", main);
