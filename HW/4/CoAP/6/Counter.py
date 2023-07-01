from coapthon.resources.resource import Resource
from coapthon.messages.message import Message



class BasicResource(Resource):
    def __init__(self, name="BasicResource", coap_server=None):
        super(BasicResource, self).__init__(name, coap_server, visible=True,
                                            observable=True, allow_children=True)

        self.counter=0


    def render_GET(self, request):
        self.payload="<<Counter is :"+str(self.counter) + ">>"
        return self


    def render_POST(self, request):
        self.counter=eval(request.payload)
        self.payload=self.counter
        return self
