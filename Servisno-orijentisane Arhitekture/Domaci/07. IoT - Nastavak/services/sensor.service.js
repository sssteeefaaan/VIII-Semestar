"use strict";

module.exports = {
    name: 'sensor',
    methods: {
        init() {
            setInterval(() => {
                let a = 20;
                let b = 40;
                let temp = Math.floor((b - a) * Math.random()) + a + this.offset;
                this.broker.emit("temperature.read", {
                    sensorId: 1,
                    temperature: temp,
                    timestamp: Date.now()
                });
            }, this.interval);
        }
    },
    events: {
        "temperature.set.1": {
            group: "other",
            handler(payload) {
                console.log("Recieved 'temparature.set.1' event in sensor service with payload: ", payload);
                this.offset = payload.offset;
            }
        }
    },
    created() {
        this.interval = 1000;
        this.offset = 0;
        this.init();
    }
}