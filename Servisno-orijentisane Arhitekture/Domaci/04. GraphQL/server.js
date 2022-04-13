const app = require('express')();
const { graphqlHTTP } = require('express-graphql');
const { schema } = require('./Schema/index.js');

const port = process.env.port || 8080;

app.use('/', graphqlHTTP({
    graphiql: true,
    schema: schema
}));

app.listen(port, () => {
    console.log(`Server is listening on http://localhost:${port}`);
});