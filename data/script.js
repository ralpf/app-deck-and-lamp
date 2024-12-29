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
    { label: 'Reddish', bgColor: '#ff1900'},
    { label: 'Cat', bgColor: '#ea00ff'},
    { label: 'Eco', bgColor: '#2cff02'},
    { label: 'Sunny', bgColor: '#fffb02'},
    { label: 'Teal', bgColor: '#02eaff'},
    { label: 'Piggy', bgColor: '#ffb390'},
    { label: 'Fire', bgColor: '#ff6f00'},
    { label: 'Flat White', bgColor: '#ffffff'}
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

// Set default mode (HSV) and attach listeners
controls.innerHTML = modes["mood"];
attachSliderListeners();
