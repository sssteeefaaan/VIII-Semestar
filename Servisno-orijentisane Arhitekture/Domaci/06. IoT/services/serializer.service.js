"use strict";

const Influx = require('influx');

module.exports = {
    name: 'serializer',
    actions: {
        read: {
            params: {
                sensorId: { type: 'number' }
            },
            async handler(ctx) {
                try {
                    const res = await this.influx.query(
                        `select * from temperature where sensorId=${ctx.params.sensorId}`
                    );
                    return res;
                } catch (err) {
                    console.log(err);
                    return null;
                }
            }
        }
    },
    events: {
        'temperature.read': {
            group: 'other',
            handler(payload) {
                console.log(
                    `Recieved 'temperature.read' event in serializer service with payload: `,
                    payload
                );

                try {
                    this.influx.writePoints([{
                        measurement: 'temperature',
                        fields: {
                            temperature: payload.temperature,
                            sensorId: payload.sensorId
                        },
                        time: payload.timestamp
                    }]);
                } catch (err) {
                    console.log(err);
                }
            }
        }
    },
    created() {
        console.log('Serializer creating... ');
        this.influx = new Influx.InfluxDB({
            host: process.env.INFLUXDB_HOST || 'influx',
            database: process.env.INFLUXDB_DATABASE || 'temperature',
            username: process.env.ADMIN_USER || 'admin',
            password: process.env.ADMIN_PASSWORD || 'admin',
            schema: [{
                measurement: 'temperature',
                fields: {
                    sensorId: Influx.FieldType.INTEGER,
                    temperature: Influx.FieldType.FLOAT
                },
                tags: ['host']
            }]
        });

        this.influx.getDatabaseNames().then((names) => {
            if (!names.includes('temperature')) {
                return this.influx.createDatabase('temperature');
            }
            return null;
        });
    }
};