const { GraphQLObjectType, GraphQLString } = require('graphql');

const DrugType = new GraphQLObjectType({
    name: 'DrugType',
    fields: {
        id: {
            type: GraphQLString
        },
        company: {
            type: GraphQLString
        },
        brand: {
            type: GraphQLString
        },
        name: {
            type: GraphQLString
        }
    }
});

module.exports = { DrugType };