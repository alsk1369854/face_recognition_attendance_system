from src.Server_manager.Bean_factory.BeanFactory import BeanFactory


class DispatcherServlet:
    def __init__(self):
        # build Bean_factory
        self.bean_factory = BeanFactory()
        pass

    def service(self, request, session, servletPath):
        print(request.args.get('test'))
        print(session['user'])
        print(servletPath)
        print(self.bean_factory.get_bean('aaa'))

        return '', request.args.get('test')
