"use strict";

/**
 * @typedef {import('moleculer').Context} Context Moleculer's Context
 */

module.exports = {
    name: "greeter",

    /**
     * Settings
     */
    settings: {

    },

    /**
     * Dependencies
     */
    dependencies: [],

    /**
     * Actions
     */
    actions: {

        /**
         * Say a 'Hello' action.
         *
         * @returns
         */
        hello: {
            rest: {
                method: "GET",
                path: "/hello"
            },
            async handler() {
                return "Hello Moleculer";
            }
        },

        /**
         * Welcome, a username
         *
         * @param {String} name - User name
         */
        welcome: {
            rest: "/welcome",
            params: {
                name: "string"
            },
            /** @param {Context} ctx  */
            async handler(ctx) {
                return `Welcomeeee, ${ctx.params.name}`;
            }
        },

        /**
         * Testing
         *
         * @param {String} randomString
         * @param {Number} randomNumber
         */
        testGET: {
            rest: {
                method: 'GET',
                path: '/test'
            },
            params: {
                randomString: "string",
                randomNumber: "string"
            },
            /** @param {Context} ctx  */
            async handler(ctx) {
                return `I received: {${ctx.params.randomString}, ${ctx.params.randomNumber}}`;
            }
        },
        testPOST: {
            rest: {
                method: 'POST',
                path: '/test'
            },
            params: {
                one: {
                    type: 'string'
                },
                two: {
                    type: 'number'
                }
            },
            /** @param {Context} ctx  */
            async handler(ctx) {
                ctx.emit('greeter.aloha', ctx.params);
                ctx.emit('products.aloha', ctx.params);
                return `I received: {${ctx.params.one}, ${ctx.params.two}}`;
            }
        }
    },

    /**
     * Events
     */
    events: {
        'greeter.aloha': {
            handler(payload) {
                console.log(payload);
            }
        }
    },

    /**
     * Methods
     */
    methods: {

    },

    /**
     * Service created lifecycle event handler
     */
    created() {
        console.log('Service created!');
    },

    /**
     * Service started lifecycle event handler
     */
    async started() {
        console.log('Service started!');
    },

    /**
     * Service stopped lifecycle event handler
     */
    async stopped() {
        console.log('Service stopped!');
    }
};