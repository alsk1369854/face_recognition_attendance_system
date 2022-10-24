from src.Server_manager.Bean_factory.BeanFactory import BeanFactory


class DispatcherServlet:
    def __init__(self):
        # build Bean_factory
        self.bean_factory = BeanFactory()
        pass

    def service(self, request, session, servlet_path):
        try:
            operate = request.args.get('operate')
            _class = self.bean_factory.get_bean(servlet_path)
            if _class:
                if hasattr(_class, operate):
                    # has method of operate
                    method = getattr(_class, operate)
                    if type(method).__name__ == 'method':
                        # Starting build method parameter values
                        parameter_quantity = method.__code__.co_argcount
                        parameter_names = method.__code__.co_varnames
                        value = {}
                        for i in range(1, parameter_quantity):
                            parameter_name = parameter_names[i]
                            if 'request' == parameter_name:
                                value['request'] = request
                            elif 'session' == parameter_name:
                                value['session'] = session
                            else:
                                value[parameter_name] = request.args.get(parameter_name)

                        # call method and return
                        v1, v2 = method(**value)
                        print(v1, v2)
                        return v1, v2

                else:
                    # has not method of operate, call index method
                    method = getattr(_class, 'index')
                    print('index')
                    v1, v2 = method()
                    print(v1, v2)
                    return v1, v2

        except Exception as error:
            print('DispatcherServlet error: ' + repr(error))



