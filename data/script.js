

//............................................................................................HANDLES

// Function to handle mode changes
function handleModeChange(mode) {
    // Hide all mode control sections
    document.querySelectorAll('.mode-controls').forEach(x => { x.style.display = 'none'; });
    // Show the selected mode's controls
    const selectedControls = document.getElementById(`${mode}-controls`);
    if (selectedControls) selectedControls.style.display = 'block';
}


// Handler Function for Button Clicks
function handleColorButtonClick(button) {
    // Get the background color from the CSS variable
    const hexColor = getComputedStyle(button).getPropertyValue('--color').trim();
    // Send the request to the backend
    fetch(`/mood?color=${encodeURIComponent(hexColor)}`).then((response) => {
        if (response.ok) console.log(`Color set to ${hexColor}`);
        else console.error(`Failed to set color: ${hexColor} ${response.status}`);
    }).catch((error) => console.error('Error:', error));
}


// Handle Flicker toggle behavior
function handleFlickerToggle(toggle) {
    const flickerFields = document.getElementById("flicker-fields");
    flickerFields.style.display = toggle.checked ? "block" : "none";
    fetch(`/mood?is_flicker=${toggle.checked ? 1 : 0}`).then((response) => {
        if (response.ok) { console.log(`Flicker set to ${toggle.checked ? "enabled" : "disabled"}`); }
        else { console.error("Failed to update flicker mode"); }
    }).catch((error) => console.error("Error:", error));
}


// Handle Flicker sliders
function handleFlickerParamChange(input, param) {
    const value = input.value;
    fetch(`/mood?${param}=${value}`).then((response) => {
        if (response.ok) console.log(`${param} set to ${value}`);
        else console.error(`Failed to update ${param}`);
    }).catch((error) => console.error(`Param ${param} Exception:`, error));
}


// Handle TV Console sliders
function handleTvConsoleSliderChange(input, param) {
    const value = input.value;
    fetch(`/tvcon?${param}=${value}`).then((response) => {
        if (response.ok) console.log(`${param} set to ${value}`);
        else console.error(`Failed to update ${param}`);
    }).catch((error) => console.error(`Param ${param} Exception:`, error));
}


// Handle Mirror Blazar toggle
function handleMirrorToggle(toggle) {
    fetch(`/tvcon?mirror=${toggle.checked ? 1 : 0}`).then((response) => {
        if (response.ok) { console.log(`Mirror Blazar set to ${toggle.checked ? "enabled" : "disabled"}`); }
        else { console.error("Failed to update mirror blazar mode"); }
    }).catch((error) => console.error("Error:", error));
}


// Set default mode
controls.innerHTML = modes["mood"];