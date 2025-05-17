
let paletteNamesLoaded = false;


const Main = {

    LoadPaletteNames() {
        // fetch from esp32 the palette names
        fetch('/tvcon/palette/names')
            .then(response => response.ok ? response.json() : Promise.reject())
            .catch(() => ["Red Lava", "Marine Blue", "Test1", "Test2"])  // simulate list
            .then(names => {
                const container = document.getElementById('palette-list');
                container.replaceChildren();
                // create buttons for each item
                names.forEach((name, idx) => {
                    const btn = document.createElement('button');
                    btn.className = 'color-button';
                    btn.textContent = name;
                    btn.onclick = () => TVConsole.OnFixedPaletteButton(idx);
                    container.appendChild(btn);
                });
            })
            .catch(err => console.error('Failed to load palette names:', err));
    },


    ModeChanged(mode) {
        // INIT - trigger pallete name fetch
        if (paletteNamesLoaded === false && mode === "mode_console") {
            paletteNamesLoaded = true;
            this.LoadPaletteNames();
        }
        // hide all mode control sections
        document.querySelectorAll('.mode-controls').forEach(x => { x.style.display = 'none'; });
        // show the selected mode's controls
        const selectedControls = document.getElementById(`${mode}-controls`);
        if (selectedControls) selectedControls.style.display = 'block';
        // first time settings apply
        if (mode === 'mode_console') TVConsole.TryApplyAllOnce();
        console.log(`Main Dropdown mode changed to ${mode}`)
    },


}; // ============================================================================== END Main



const Blazar = {

    OnColorButton(button) {
        const hexColor = getComputedStyle(button).getPropertyValue('--color').trim();
        do_fetch(`/mood?color=${encodeURIComponent(hexColor)}`);
    },


    FlickerToggle(toggle) {
        const flickerFields = document.getElementById("flicker-fields");
        flickerFields.style.display = toggle.checked ? "block" : "none";
        do_fetch(`/mood?is_flicker=${toggle.checked ? 1 : 0}`);
    },


    FlickerParamChange(input, param) {
        do_fetch(`/mood?${param}=${input.value}`);
    }

}; // ============================================================================ END Blazar



const TVConsole = {

    isInited: false,
    palette_idx_selected: 0,
    control_hue: 0,             // ui8
    control_sat: 255,           // ui8
    randEnable: 0,              // toggle


    TryApplyAllOnce() {
        if (this.isInited) return;
        this.isInited = true;
        // find elements
        const body = document.getElementById("mode_console-controls");
        const dropdown = body.querySelector("#submode_console");
        const bright   = body.querySelector("#val-channel");
        const blend    = body.querySelector("#blend-speed");
        // init local globals
        let request = `/tvcon?blend=${blend.value}&bright=${bright.value}`;
        do_fetch(request);
        // init local variables
        this.control_hue = body.querySelector("#hue-channel").value;
        this.control_sat = body.querySelector("#sat-channel").value;
        // trigger dropdown
        this.OnSubmodeChanged(dropdown.value);
    },


    OnSubmodeChanged(submode) {
        const parent = document.getElementById('mode_console-controls');
        // Hide all divs with id starting with "submode_"
        Array.from(parent.children).forEach(child => {
            if (child.id?.startsWith('submode_'))
                child.style.display = 'none';
        });
        // Show the one that matches the selected submode
        const target = document.getElementById(submode);
        if (target) target.style.display = 'block';
        console.log(`TVConsole change submode = ${submode}`)
        // Call submode parameters
        let request = "";
        if (submode === "submode_mirror_blazar") request = `/tvcon/mirror`; else
        if (submode === "submode_palette")       request = `/tvcon/palette?idx=${this.palette_idx_selected}&irand=${this.randEnable}`; else
        if (submode === "submode_hsv")           request = `/tvcon/hsv?hue=${this.control_hue}&sat=${this.control_sat}`;
        do_fetch(request);
    },


    OnFixedPaletteButton(idx) {
        this.palette_idx_selected = idx;
        do_fetch(`/tvcon/palette?idx=${idx}`);
    },


    OnNamedPaletteButton(name) {
        do_fetch(`/tvcon/palette?name=${encodeURIComponent(name)}`);
    },


    OnParamChange(input, subroute, param) {
        const value = input.value;
        const route = subroute ? `/tvcon/${subroute}` : `/tvcon`;
        const request = `${route}?${param}=${value}`;
        do_fetch(request);
        // fill local variables
        if (subroute === 'hsv') {
            if (param === 'hue') this.control_hue = value;
            if (param === 'sat') this.control_sat = value;
        }
    },


    OnToggle(toggle) {
        this.randEnable = toggle.checked ? 1 : 0;
        do_fetch(`/tvcon/palette?irand=${this.randEnable}`);
    },

}; // ======================================================================== END TVConsole


// helpers

function do_fetch(request) {
    fetch(request).then((response) => {
        if (response.ok) console.log(`Request OK ${request}`);
        else console.error(`FAIL Reques ${request}`);
    }).catch((error) => console.error(`Exception on ${request}`, error));
}

// run on document laod
document.addEventListener("DOMContentLoaded", function() {
    Main.ModeChanged(document.getElementById('main-dropdown').value);
});

// Set default mode
//controls.innerHTML = modes["mood"];