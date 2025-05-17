
let paletteNamesLoaded = false;


const Main = {

    LoadPaletteNames() {
        // fetch from esp32 the palette names
        fetch('/tvcon/palette_names')
            .then(response => response.json())
            .then(names => {
                const container = document.getElementById('palette-list');
                container.replaceChildren();
                // create buttons for each item
                names.forEach(name => {
                    const btn = document.createElement('button');
                    btn.className = 'color-button';
                    btn.textContent = name;
                    btn.onclick = () => TVConsole.OnNamedPaletteButton(name);
                    container.appendChild(btn);
                });
            })
            .catch(err => console.error('Failed to load palette names:', err));
    },


    ModeChanged(mode) {
        // hide all mode control sections
        document.querySelectorAll('.mode-controls').forEach(x => { x.style.display = 'none'; });
        // show the selected mode's controls
        const selectedControls = document.getElementById(`${mode}-controls`);
        if (selectedControls) selectedControls.style.display = 'block';
        // trigger pallete name fetch
        if (paletteNamesLoaded === false && mode === "mode_console") {
            paletteNamesLoaded = true;
            Main.LoadPaletteNames();
        }
    }

}; // ============================================================================== END Main



const Blazar = {

    OnColorButton(button) {
        // Get the background color from the CSS variable
        const hexColor = getComputedStyle(button).getPropertyValue('--color').trim();
        // Send the request to the backend
        fetch(`/mood?color=${encodeURIComponent(hexColor)}`).then((response) => {
            if (response.ok) console.log(`Color set to ${hexColor}`);
            else console.error(`Failed to set color: ${hexColor} ${response.status}`);
        }).catch((error) => console.error('Error:', error));
    },


    FlickerToggle(toggle) {
        const flickerFields = document.getElementById("flicker-fields");
        flickerFields.style.display = toggle.checked ? "block" : "none";
        fetch(`/mood?is_flicker=${toggle.checked ? 1 : 0}`).then((response) => {
            if (response.ok) { console.log(`Flicker set to ${toggle.checked ? "enabled" : "disabled"}`); }
            else { console.error("Failed to update flicker mode"); }
        }).catch((error) => console.error("Error:", error));
    },


    FlickerParamChange(input, param) {
        const value = input.value;
        fetch(`/mood?${param}=${value}`).then((response) => {
            if (response.ok) console.log(`${param} set to ${value}`);
            else console.error(`Failed to update ${param}`);
        }).catch((error) => console.error(`Param ${param} Exception:`, error));
    }

}; // ============================================================================ END Blazar



const TVConsole = {

    OnSubmodeChanged(submode) {
        const parent = document.getElementById('mode_console-controls');
        // Hide all divs with id starting with "submode_"
        Array.from(parent.children).forEach(child => {
            if (child.id?.startsWith('submode_'))
                child.style.display = 'none';
        });

        // Show the one that matches the selected submode
        const target = document.getElementById(submode);
        if (target)
            target.style.display = 'block';
    },


    OnSliderChange(input, param) {
        const value = input.value;
        fetch(`/tvcon?${param}=${value}`).then((response) => {
            if (response.ok) console.log(`${param} set to ${value}`);
            else console.error(`Failed to update ${param}`);
        }).catch((error) => console.error(`Param ${param} Exception:`, error));
    },


    OnFixedPaletteButton(button) {
        const index = button.dataset.idx;
        fetch(`/tvcon/palette?idx=${index}`).then((response) => {
            if (response.ok) console.log(`Palette index set to ${index}`);
            else console.error(`Failed to set Palette index: ${index} ${response.status}`);
        }).catch((error) => console.error('Error:', error));
    },


    OnNamedPaletteButton(name) {
        fetch(`/tvcon/palette?name=${encodeURIComponent(name)}`)
            .then(res => {
                if (res.ok) console.log(`Palette set by name to ${name}`);
                else console.error(`Failed to set palette by name: ${name}`);
            })
            .catch(err => console.error('Palette set by name error:', err));
    },


    OnParamChange(input, param) {
        const value = input.value;
        fetch(`/tvcon/palette?${param}=${value}`).then((response) => {
            if (response.ok) console.log(`Palette param ${param} set to ${value}`);
            else console.error(`Failed to update Palette param ${param}`);
        }).catch((error) => console.error(`Param ${param} Exception:`, error));
    }

}; // ======================================================================== END TVConsole



// Set default mode
//controls.innerHTML = modes["mood"];