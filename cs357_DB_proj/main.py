""" Main page using webpy 0.3 """
import web
import model

### Url mappings

urls = (
    '/', 'Index',
    '/view/(\d+)', 'View',
    '/signup', 'Signup',
    '/delete/(\d+)', 'Delete',
    '/edit/(\d+)', 'Edit',
    '/visit', 'Visit',
    '/editvisitor/(.+)/(\d+)', 'EditVisitor',
    '/deletevisitor/(.+)/(\d+)', 'DeleteVisitor',
    '/bookhouse', 'Book',
    '/houseid/(\d+)', 'BookEdit',
    '/bookhouse/succeed', 'Succeed',
    '/bookhouse/fail', 'Fail',
    '/orderinfo/(\d+)', 'OrderInfo',
    '/noorder', 'NoOrder',
    '/deleteorder/(\d+)', 'DeleteOrder',
    '/agencies', 'Agencies',
    '/agency/(\d+)', 'Agency',
    '/delete/agency/(\d+)', 'DeleteAgency',
    '/houses', 'House',
    '/delete/aghashouse/(\d+)', 'Delaghashouse',
    '/edithouse/(\d+)', 'Edithouse',
    '/deletehouse/(\d+)', 'DeleteHouse',
)


### Templates
t_globals = {
    'datestr': web.datestr
}
render = web.template.render('/home/henry/database/webpy/cs357proj/templates', base='base', globals=t_globals)

class House:
    form = web.form.Form(
        web.form.Textbox('price'),
        web.form.Textbox('type'),
        web.form.Textbox('year'),
        web.form.Textbox('locationHouse'),
        web.form.Textbox('capacity'),
        web.form.Button('Add New House'),
        validators = [web.form.Validator("location is same as capacity", lambda i: i.locationHouse != i.capacity)]
        )
    def GET(self):
        form=self.form()
        houses = model.get_Allhouses()
        return render.allhouses(houses, form)
    def POST(self):
        form=self.form()
        houses = model.get_Allhouses()
        if not form.validates():
            return render.allhouses(houses,form)
        model.new_house(form.d.price, form.d.type, form.d.year, form.d.locationHouse, form.d.capacity)
        raise web.seeother('/houses')
class Edithouse:
    def GET(self, hsid):
        form=House.form()
        house=model.get_house(hsid)
        form.fill(house)
        return render.editindihouse(house, form)
    def POST(self, hsid):
        form=House.form()
        house=model.get_house(hsid)
        if not form.validates():
            return render.editindihouse(house, form)
        model.update_house(hsid, form.d.price, form.d.type, form.d.year, form.d.locationHouse, form.d.capacity)
        raise web.seeother('/houses')

class DeleteHouse:
    def POST(self, hsid):
        model.del_house(hsid)
        raise web.seeother('/houses')
class Edit:

    def GET(self, id):
        client = model.get_client(int(id))
        form = Signup.form()
        form.fill(client)
        return render.edit(client, form)

    def POST(self, id):
        form = Signup.form()
        client = model.get_client(int(id))
        if not form.validates():
            return render.edit(client, form)
        model.update_client(int(id), form.d.Name, form.d.password, form.d.Email, form.d.Credit_Card_No, form.d.Phone_No, form.d.SEX)
        raise web.seeother('/')      

class Agencies:
    form = web.form.Form(
        web.form.Textbox('Telephone'),
        web.form.Textbox('location'),
        web.form.Button('Add New Agency'),
        validators = [web.form.Validator("telephone.", lambda i: i.location != i.Telephone)]
    )
    def GET(self):
        form=self.form()
        agencies = model.get_agencies()
        return render.agencies(agencies, form)
    def POST(self):
        form = self.form()
        agencies = model.get_agencies()
        if not form.validates():
            return render.agencies(agencies,form)
        model.new_agency(form.d.Telephone, form.d.location)
        raise web.seeother('/agencies')

class Agency:
    form = web.form.Form(
        web.form.Textbox('Telephone'),
        web.form.Textbox('location'),
        web.form.Textbox('Choose_House'),
        web.form.Button('Edit'),
        validators = [web.form.Validator("telephone.", lambda i: i.location != i.Telephone)]
    )
    def GET(self, agid):
        agency = model.get_agency(agid)
        agencyhashouse = model.get_aghashouse(agid)
        print agencyhashouse
        form = self.form()
        form.fill(agency)
        return render.showAgency(agency, form, agencyhashouse)
    def POST(self, agid):
        form = self.form()
        agencyhashouse = model.get_aghashouse(agid)
        agency = model.get_agency(agid)
        if not form.validates():
            raise web.seeother('/agencies')
        if model.hsoccupied(form.d.Choose_House)==0:
            model.update_agency(agid, form.d.Telephone, form.d.location)
            model.agencywantshouse(agid, form.d.Choose_House)
        raise web.seeother('/agencies')


class Index:

    def GET(self):
        clients = model.get_clients()
        return render.index(clients)

class View:
    def GET(self, id):
        client = model.get_client(id)
        return render.view(client)

class Succeed:
    def GET(self):
        return render.succeed()
class Fail: 
    def GET(self):
        return render.fail()
class NoOrder:
    def GET(self):
        return render.noorder()
class Signup:

    form = web.form.Form(
        web.form.Textbox('Name'),
        web.form.Password('password'),
        web.form.Password('password_again'),
        web.form.Textbox('Email'),
        web.form.Textbox('Credit_Card_No'),
        web.form.Textbox('Phone_No'),
        web.form.Dropdown('SEX', ['m', 'f']),
        web.form.Button('Submit'),
        validators = [web.form.Validator("Passwords didn't match.", lambda i: i.password == i.password_again)]
    )

    def GET(self):
        form = self.form()
        return render.signup(form)

    def POST(self):
        form = self.form()
        if not form.validates():
            return render.signup(form)
        if model.exist(form.d.Name) == 0:
            model.new_client(form.d.Name, form.d.password, form.d.Email, form.d.Credit_Card_No, form.d.Phone_No, form.d.SEX)
        raise web.seeother('/')

class Delete:
    def POST(self, id):
        model.del_client(int(id))
        raise web.seeother('/')

class DeleteAgency:
    def POST(self, id):
        model.del_agency(id)
        raise web.seeother('/agencies')

class DeleteVisitor: 
    def POST(self, name, date):
        a = model.del_visitor(name, date)
        raise web.seeother('/visit')

class Delaghashouse:
    def GET(self, idhs):
        model.del_aghashouse(idhs)
        raise web.seeother('/agencies')

class DeleteOrder:

    def POST(self, hsid):
        model.order_delete(hsid)
        raise web.seeother('/bookhouse')

class EditVisitor:
    def GET(self, name, date):
        visitor = model.get_visitor(name)
        form = Visit.form()
        form.fill(visitor)
        return render.editvisitor(visitor, form)
    def POST(self, name, date):
        form = Visit.form()
        client = model.get_visitor(name)
        if not form.validates():
            return render.editvisitor(client, form)
        if model.exist(form.d.Name)==1:
            ag=model.get_agencyID(form.d.HouseID)
            print ag
            hs=model.get_house(form.d.HouseID)
            print hs
            model.del_visitor(name,date)
            model.new_visit(form.d.Name, form.d.Visit_Date, form.d.HouseID, hs.locationHouse, ag.Agency_idAgency)
        raise web.seeother('/visit')

class BookEdit:
    def GET(self, hsID):
        form = Book.form()
        house = model.get_house(hsID)
        form.fill(house)
        return render.editHouse(house, form)
    def POST(self, hsID):
        form = Visit.form()
        house = model.get_house(hsID)
        if not form.validates():
            return render.editHouse(house, form)

class OrderInfo:
    def GET(self, hsID):
        form = Book.form()
        order = model.get_order(hsID)
        form.fill(order)
        if order!=None:
            return render.orderinfo(order, form)
        else:
            raise web.seeother('/noorder')
    def POST(self, hsID):
        form = Book.form()
        if not form.validates():
            return render.house(form)
        if model.existHs(form.d.idHouse)==1 and model.exist(form.d.Name)==1 and model.hs_booked(form.d.Name, form.d.idHouse)==1:
            model.order_update(form.d.Name, form.d.idHouse, form.d.Duration)
            raise web.seeother('/bookhouse/succeed')
        else:
            raise web.seeother('/bookhouse/fail')

class Visit:
    form = web.form.Form(
        web.form.Textbox('Name'),
        web.form.Textbox('Visit_Date'),
        web.form.Textbox('HouseID'),
        web.form.Button('Submit'),
    )

    def GET(self):
        form = self.form()
        visitors = model.get_visitors()
        return render.visit(form, visitors)
    def POST(self):
        form = self.form()
        if not form.validates():
            return render.visit(form)
        if model.exist(form.d.Name)==1 and model.existHs(form.d.HouseID):
            ag=model.get_agencyID(form.d.HouseID)
            hs=model.get_house(form.d.HouseID)
            model.new_visit(form.d.Name, form.d.Visit_Date, form.d.HouseID, hs.locationHouse, ag.Agency_idAgency)
        raise web.seeother('/visit')
class Book:
    form = web.form.Form(
        web.form.Textbox('Name'),
        web.form.Textbox('idHouse'),
        web.form.Textbox('Duration'),
        web.form.Button('Book it!'),
    )
    def GET(self):
        form = self.form()
        houses = model.get_houses()
        return render.house(form, houses)
    def POST(self):
        form = self.form()
        if not form.validates():
            return render.house(form)
        if model.existHs(form.d.idHouse)==1 and model.exist(form.d.Name)==1 and model.hs_booked(form.d.Name, form.d.idHouse)==0:
            houseinfo = model.get_house(form.d.idHouse)
            cost = houseinfo.price * int(form.d.Duration)
            model.house_book(form.d.Name, form.d.idHouse, form.d.Duration, cost)
            raise web.seeother('/bookhouse/succeed')
        else:
            raise web.seeother('/bookhouse/fail')

app = web.application(urls, globals())

if __name__ == '__main__':
    app.run()