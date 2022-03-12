const grpc = require('@grpc/grpc-js');
const { addEmailService } = require('./services/email-service');

const server = new grpc.Server();
addEmailService(server);

server.bindAsync('localhost:5000', grpc.ServerCredentials.createInsecure(), () => {
    server.start();
    console.log('Server running on http://localhost:5000');
});