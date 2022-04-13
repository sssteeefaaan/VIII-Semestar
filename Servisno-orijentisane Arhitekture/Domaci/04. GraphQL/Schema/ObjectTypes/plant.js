const { GraphQLObjectType, GraphQLString, GraphQLInt } = require('graphql');

const PlantType = new GraphQLObjectType({
    name: 'PlantType',
    fields: {
        id: {
            type: GraphQLInt
        },
        commonName: {
            type: GraphQLString
        },
        family: {
            type: GraphQLString
        },
        scientificName: {
            type: GraphQLString
        }
    }
});

module.exports = { PlantType };