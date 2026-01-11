## ESP32 program for Blazar project

### How to build
+ after each frontend build, the index.html has to be reuploaded because of hashed names. They are required for chaching burst
+ replace HTML code in data/index.html  
+ fix paths so it's starts with https://ralpf.github.io for ex
  ```
    <link rel="icon" href="https://ralpf.github.io/web-blazar/assets/icon-DiSFa2Td.png"/>
    <script type="module" crossorigin src="https://ralpf.github.io/web-blazar/assets/index-CT3mzoxI.js"></script>
    <link rel="stylesheet" crossorigin href="https://ralpf.github.io/web-blazar/assets/index-IZ_GRBuC.css">
  ```
+ build upload firmware to esp `pio run -t upload`
+ upload filesystem image that contains index.html `pio run -t uploadfs`