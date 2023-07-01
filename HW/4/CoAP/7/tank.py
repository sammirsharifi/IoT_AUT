from coapthon.resources.resource import Resource


class BasicResource(Resource):
    def __init__(self, name="tank", coap_server=None):
        super(BasicResource, self).__init__(name, coap_server, visible=True,
                                            observable=True, allow_children=True)
        self.level = 0

    def render_GET(self, request):
        self.payload = f"<< tank level is {self.level} >>"
        return self

    def render_POST(self, request):
        self.check_and_increase(int(request.payload))
        return self

    def check_and_increase(self, num):
        if not (num <= 0 or num > 20):
            self.level = self.level + num
            if self.level > 100:
                self.level = 100
