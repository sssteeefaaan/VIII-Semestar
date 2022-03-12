from tokenize import Name
import grpc
import greet_pb2_grpc as greetRPC
import greet_pb2 as greetRPC2

channel = grpc.insecure_channel('localhost:5000')
stub = greetRPC.GreeterStub(channel)

reply = stub.SayHello(greetRPC2.HelloRequest(name="Stefan"))
print(reply)