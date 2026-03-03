#!/usr/bin/env node
const path = require("path");
const Splash = require("./splash.js"); // your rendering engine

const pagesFolder = path.join(__dirname, "cannnddyy/pages");

const browser = new Splash({
    defaultPage: path.join(pagesFolder, "setup.html"),
    pagesFolder: pagesFolder,
    blockedSites: [],
    timeLimits: {},      // per site time limits
    tabs: [],
});

// load candy:// logic
browser.on("loadUrl", (url, tab) => {
    if (url.startsWith("candy://")) {
        const page = url.replace("candy://","") + ".html";
        tab.loadPage(path.join(browser.pagesFolder, page));
    }
});

// start the browser
browser.run();
