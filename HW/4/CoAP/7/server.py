from coapthon.server.coap import CoAP
from tank import BasicResource


class CoAPServer(CoAP):
    def __init__(self, host, port):
        CoAP.__init__(self, (host, port))
        self.add_resource('tank/', BasicResource())


def main():
    print("hello coap")
    server = CoAPServer("0.0.0.0", 5683)
    try:
        server.listen(10)
    except KeyboardInterrupt:
        print("Server Shutdown")
        server.close()
        print("Exiting...")


if __name__ == '__main__':
    main()
