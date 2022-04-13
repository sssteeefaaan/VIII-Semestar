"use strict";
const os = require('os');

module.exports = {
    nodeID: (process.env.NODEID ? process.env.NODEIS + "-" : "") + os.hostname().toLowerCase()
};