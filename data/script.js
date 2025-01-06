const controls = document.getElementById("controls");
const modeSelector = document.getElementById("mode");

// Function to generate a slider row
function drawSliderRow(labelText, id, min = 0, max = 100, initialValue = min) {
  return `
    <div class="slider-row">
      <label for="${id}">${labelText}:</label>
      <span id="${id}-value">${initialValue}</span>
      <input type="range" id="${id}" min="${min}" max="${max}" value="${initialValue}">
    </div>
  `;
}

// Define controls for each mode
const modes = {
  mood: drawButtonGrid([
    { label: 'Ruby', bgColor: '#ff0000'},
    { label: 'Cat', bgColor: '#ff00ea'},
    { label: 'Fire', bgColor: '#ffa000'},
    { label: 'Eco', bgColor: '#2cff02'},
    { label: 'Teal', bgColor: '#02eaff'},
    { label: 'Sunny', bgColor: '#ff9b02'},
    { label: 'Flat White', bgColor: '#ffffff'},
    { label: 'OFF', bgColor: '#000000', textColor: '#ffffff'}
  ])
  ,
  hsv: `
    ${drawSliderRow("Hue", "h", 0, 360)}
    ${drawSliderRow("Saturation", "s", 0, 100)}
    ${drawSliderRow("Value", "v", 0, 100)}
  `,
  rgb: `
    ${drawSliderRow("Red Channel", "r", 0, 255)}
    ${drawSliderRow("Green Channel", "g", 0, 255)}
    ${drawSliderRow("Blue Channel", "b", 0, 255)}
  `,
  other: `
    <div class="slider-row">
      <label for="color">Color Picker:</label>
      <input type="color" id="color">
    </div>
    <div class="slider-row">
      <label for="float">Float Value:</label>
      <input type="number" id="float" step="0.01">
    </div>
    <div class="slider-row">
      <label for="text">Text Parameter:</label>
      <input type="text" id="text">
    </div>
  `
};


// Add Flicker toggle and additional fields
function addFlickerControls() {
  return `
    <div class="flicker-controls">
      <label class="flicker-toggle">
        Flicker<input type="checkbox" id="flicker-toggle" onclick="handleFlickerToggle(this)">
      </label>
      <div id="flicker-fields" style="display: none;">
        <div class="slider-row">
          <label for="flicker-intensity">Hue Amplitude8</label>
          <input type="number" id="flicker-intensity" min="1" max="100" value="50">
        </div>
        <div class="slider-row">
          <label for="flicker-speed">Hue TimeScale16</label>
          <input type="number" id="flicker-speed" min="1" max="100" value="50">
        </div>
      </div>
    </div>
  `;
}


// Update mood mode to include Flicker controls
modes.mood += addFlickerControls();



// Update controls dynamically based on selected mode
modeSelector.addEventListener("change", (e) => {
  const selectedMode = e.target.value;
  controls.innerHTML = modes[selectedMode];
  attachSliderListeners(); // Reattach listeners after mode change
});

// Attach value display updates for sliders
function attachSliderListeners() {
  document.querySelectorAll('input[type="range"]').forEach(slider => {
    slider.addEventListener("input", (e) => {
      const valueSpan = document.getElementById(`${e.target.id}-value`);
      valueSpan.textContent = e.target.value; // Update value display
    });
  });
}

// Function to generate a button grid
function drawButtonGrid(buttons) {
  let buttonsHTML = '';
  buttons.forEach((button) => {
    const { label, bgColor, textColor } = button;
    buttonsHTML += `
      <button class="color-button" 
        style="background-color: ${bgColor}; color: ${textColor};"
        onclick="handleColorButtonClick('${bgColor}')">${label}
      </button>`;
  });
  return `<div class="button-grid">${buttonsHTML}</div>`;
}

// Handler Function for Button Clicks
function handleColorButtonClick(color) {
  fetch(`/mood?color=${encodeURIComponent(color)}`)
    .then((response) => {
      if (response.ok) { console.log(`Color set to ${color}`); }
      else { console.error(`Failed to set color: ${response.status}`); }
    })
    .catch((error) => console.error('Error:', error));
}


// Handle Flicker toggle behavior
function handleFlickerToggle(toggle) {
  const flickerFields = document.getElementById("flicker-fields");
  flickerFields.style.display = toggle.checked ? "block" : "none";

  // Call /mood?is_flicker=<1 or 0>
  fetch(`/mood?is_flicker=${toggle.checked ? 1 : 0}`).then((response) => {
      if (response.ok) { console.log(`Flicker set to ${toggle.checked ? "enabled" : "disabled"}`); }
      else { console.error("Failed to update flicker mode"); }
    })
    .catch((error) => console.error("Error:", error));
}



// Set default mode (HSV) and attach listeners
controls.innerHTML = modes["mood"];
attachSliderListeners();
