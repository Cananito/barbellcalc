/**
 * Globals.
 */
let platesToWeightOption;
let weightToPlatesOption;
let inputTextField;
let resultLabel;
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

function inputTextFieldEventHandler(event) {
  const exports = wasmObj.instance.exports;
  const textInputStringBuffer = exports.js_text_input_string_buffer;
  writeStringToWASMMemoryBuffer(inputTextField.value, textInputStringBuffer);
  let resultPointer;
  if (platesToWeightOption.checked) {
    resultPointer = exports.calc_plates_to_weight(textInputStringBuffer);
  } else {
    // TODO: Update to the right function!
    resultPointer = exports.calc_plates_to_weight(textInputStringBuffer);
  }
  const result = stringFromNullTerminatedCCharPointer(resultPointer);
  resultLabel.innerHTML = "Result: " + result;
}

function platesToWeightOptionEventHandler(event) {
  weightToPlatesOption.checked = false;
}

function weightToPlatesOptionEventHandler(event) {
  platesToWeightOption.checked = false;
}

function initializeUI() {
  const body = document.body;

  let conversionFieldset = document.createElement("fieldset");
  platesToWeightOption = document.createElement("input");
  platesToWeightOption.type = "radio";
  platesToWeightOption.checked = true;
  platesToWeightOption.oninput = platesToWeightOptionEventHandler;
  let platesToWeightOptionLabel = document.createElement("label");
  platesToWeightOptionLabel.innerHTML = "Plates to weight";
  weightToPlatesOption = document.createElement("input");
  weightToPlatesOption.type = "radio";
  weightToPlatesOption.checked = false;
  weightToPlatesOption.oninput = weightToPlatesOptionEventHandler;
  let weightToPlatesOptionLabel = document.createElement("label");
  weightToPlatesOptionLabel.innerHTML = "Weight to plates";
  conversionFieldset.appendChild(platesToWeightOption);
  conversionFieldset.appendChild(platesToWeightOptionLabel);
  conversionFieldset.appendChild(weightToPlatesOption);
  conversionFieldset.appendChild(weightToPlatesOptionLabel);
  body.appendChild(conversionFieldset);

  body.appendChild(document.createElement("br"));

  inputTextField = document.createElement("input");
  inputTextField.type = "text";
  inputTextField.oninput = inputTextFieldEventHandler;
  body.appendChild(inputTextField);

  body.appendChild(document.createElement("br"));
  body.appendChild(document.createElement("br"));

  resultLabel = document.createElement("span");
  body.appendChild(resultLabel);
}

/**
 * main.
 */

function main() {
  initializeUI();
  const wasmPromise = WebAssembly.instantiateStreaming(fetch("calc.wasm"), {});
  wasmPromise.then(function (obj) {
    wasmObj = obj;
    inputTextFieldEventHandler(null);
  });
};
document.addEventListener("DOMContentLoaded", main);
