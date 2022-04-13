"use strict";

module.exports = {
    name: 'actuator',
    actions: {
        set: {
            params: {
                offset: { type: 'number' },
                id: { type: 'number' }
            },
            async handler(ctx) {
                console.log(ctx.params);
                this.broker.emit(`temperature.set.${ctx.params.id}`, ctx.params);
                return 'Success!';
            }
        }
    }
}