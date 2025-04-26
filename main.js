// TODO: Make it installable (PWA).

// TODO: Add offline support.

// TODO: Add URL to GitHub repo.

// TODO: Maximize UI size, but keep in mind mobile keyboard.

/**
 * Globals.
 */
let platesTextField;
let weightTextField;
let wasmObj;


/**
 * WASM helper functions.
 */
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


/**
 * Event handlers.
 */
function platesTextFieldEventHandler(event) {
  const input = platesTextField.value;
  if (input.length > 58) {
    // Prevent overflow of js_text_input_string_buffer;
    weightTextField.value = "";
    return;
  }
  const exports = wasmObj.instance.exports;
  const textInputStringBuffer = exports.js_text_input_string_buffer;
  writeStringToWASMMemoryBuffer(input, textInputStringBuffer);
  const resultPointer = exports.calc_plates_to_weight(textInputStringBuffer);
  const result = stringFromNullTerminatedCCharPointer(resultPointer);
  weightTextField.value = result;
}

function weightTextFieldEventHandler(event) {
  const input = weightTextField.value;
  if (input.length > 58) {
    // Prevent overflow of js_text_input_string_buffer;
    platesTextField.value = "";
    return;
  }
  const exports = wasmObj.instance.exports;
  const textInputStringBuffer = exports.js_text_input_string_buffer;
  writeStringToWASMMemoryBuffer(input, textInputStringBuffer);
  const resultPointer = exports.calc_weight_to_plates(textInputStringBuffer);
  const result = stringFromNullTerminatedCCharPointer(resultPointer);
  platesTextField.value = result;
}

function setDefaultStyleOnLabel(label) {
  label.style.fontSize = "min(8vh, 8vw)";
}

function setDefaultStyleOnTextField(textField) {
  textField.type = "text";
  textField.style.width = "100%";
  textField.style.height = "min(25vh, 25vw)";
  textField.style.fontSize = "min(20vh, 20vw)";
  textField.style.padding = "1px";
}

function initializeUI() {
  const body = document.body;

  platesLabel = document.createElement("span");
  platesLabel.innerHTML = "Plates (lbs): ";
  setDefaultStyleOnLabel(platesLabel);
  body.appendChild(platesLabel);

  body.appendChild(document.createElement("br"));

  platesTextField = document.createElement("input");
  platesTextField.oninput = platesTextFieldEventHandler;
  setDefaultStyleOnTextField(platesTextField);
  body.appendChild(platesTextField);

  body.appendChild(document.createElement("br"));
  body.appendChild(document.createElement("br"));

  weightLabel = document.createElement("span");
  weightLabel.innerHTML = "Weight (lbs): ";
  setDefaultStyleOnLabel(weightLabel);
  body.appendChild(weightLabel);

  body.appendChild(document.createElement("br"));

  weightTextField = document.createElement("input");
  weightTextField.oninput = weightTextFieldEventHandler;
  setDefaultStyleOnTextField(weightTextField);
  body.appendChild(weightTextField);
}

/**
 * main.
 */

function main() {
  initializeUI();
  const wasmPromise = WebAssembly.instantiateStreaming(fetch("calc.wasm"), {});
  wasmPromise.then(function (obj) {
    wasmObj = obj;
    platesTextFieldEventHandler(null);
  });
};
document.addEventListener("DOMContentLoaded", main);
