const PROTO_PATH = __dirname + '/../../Protos/email.proto';

const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');

const packageDefinition = protoLoader.loadSync(
    PROTO_PATH, {
        keepCase: true,
        longs: String,
        enums: String,
        defaults: true,
        oneofs: true
    }
);

const protoDescriptor = grpc.loadPackageDefinition(packageDefinition);

const addEmailService = (server) => {
    server.addService(protoDescriptor.email.Email.service, {
        register: register,
        sendEmail: sendEmail,
        checkMailbox: checkMailbox
    });
};

module.exports = {
    addEmailService
};


// Implementation
const users = {};

function register(call, callback) {
    if (!call.request.email || !call.request.password) {
        callback(null, {
            code: 401,
            message: "Wrong credentials!"
        });
    } else if (users[call.request.email] != null) {
        callback(null, {
            code: 401,
            message: "Email already registered!"
        });
    } else {
        users[call.request.email] = {
            credentials: call.request,
            unread: []
        };

        callback(null, {
            code: 200,
            message: "User successfully registered!"
        });
    }
};

function sendEmail(call, callback) {
    if (!call.request.to_user || !call.request.from_user || !call.request.message) {
        callback(null, {
            code: 401,
            message: "Wrong email format!"
        });
    } else if (!users[call.request.to_user]) {
        callback(null, {
            code: 401,
            message: "Receiver not registered!"
        });
    } else if (!users[call.request.from_user]) {
        callback(null, {
            code: 401,
            message: "Sender not registered!"
        });
    } else {
        users[call.request.to_user].unread.push(call.request);

        callback(null, {
            code: 200,
            message: "Email sent successfully!"
        });
    }
};

function checkMailbox(call) {
    if (!call.request.email || !call.request.password)
        return call.end();

    const u = users[call.request.email];
    if (!u || u.credentials.password != call.request.password)
        return call.end();

    u.unread.forEach(email => call.write(email));
    return call.end();
};