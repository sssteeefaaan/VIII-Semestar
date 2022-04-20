"use strict";

module.exports = {
    name: 'actuator',
    actions: {
        set: {
            params: {
                offset: { type: 'number' },
                sensorId: { type: 'number' }
            },
            async handler(ctx) {
                console.log(ctx.params);
                this.broker.emit(`temperature.set.${ctx.params.sensorId}`, ctx.params);
                return 'Success setting temperature!';
            }
        }
    }
}