const { GraphQLSchema, GraphQLObjectType, GraphQLList, GraphQLInt, GraphQLString } = require('graphql');
const uuid = require('uuid');

const { PlantType } = require('./ObjectTypes/plant');
const { DrugType } = require('./ObjectTypes/drug');

const data_plants = require('../data/mock_data_plants.json');
const data_drugs = require('../data/mock_data_drugs.json');

const RootQuery = new GraphQLObjectType({
    name: 'RootQuery',
    fields: {
        getPlants: {
            type: new GraphQLList(PlantType),
            args: {},
            resolve: (parent, args) => {
                return data_plants;
            }
        },
        getPlant: {
            type: PlantType,
            args: { id: { type: GraphQLInt } },
            resolve: (parent, args) => {
                return data_plants.find(p => p.id == args.id);
            }
        },
        getDrugs: {
            type: new GraphQLList(DrugType),
            args: {},
            resolve: (parent, args) => {
                return data_drugs;
            }
        },
        getDrug: {
            type: DrugType,
            args: {
                id: { type: GraphQLString }
            },
            resolve: (parent, args) => {
                return data_drugs.find(d => d.id == args.id);
            }
        }
    }
});

const RootMutation = new GraphQLObjectType({
    name: 'RootMutation',
    fields: {
        createPlant: {
            type: PlantType,
            args: {
                commonName: { type: GraphQLString },
                family: { type: GraphQLString },
                scientificName: { type: GraphQLString }
            },
            resolve: (parent, args) => {
                const new_plant = { id: data_plants.length, commonName: args.commonName, family: args.family, scientificName: args.scientificName };
                data_plants.push(new_plant);
                return new_plant;
            }
        },
        createDrug: {
            type: DrugType,
            args: {
                company: { type: GraphQLString },
                brand: { type: GraphQLString },
                name: { type: GraphQLString }
            },
            resolve: (parent, args) => {
                const new_drug = { id: uuid.v1(), company: args.company, brand: args.brand, name: args.name };
                data_drugs.push(new_drug);
                return new_drug;
            }
        }
    }
});

const schema = new GraphQLSchema({ mutation: RootMutation, query: RootQuery });

module.exports = { schema };