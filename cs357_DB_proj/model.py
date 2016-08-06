import web, datetime

db = web.database(dbn='mysql', db='mydb_rent', user='root', pw='123456')
def existHs(hsid):
	try:
		db.select('House', where="idHouse=$hsid", vars=locals())[0]
		return 1
	except IndexError:
		return 0
def exist(name):
	try:
		db.select('Client', where="username=$name", vars=locals())[0]
		return 1
	except IndexError:
		return 0
def hsoccupied(hsid):
	try:
		db.select('Agency_has_House', where='idHouse=$hsid', vars=locals())[0]
		return 1
	except Exception:
		return 0	
def new_agency(tele, location):
	AgencyID = 0
	agencies = get_agencies()
	for agency in agencies:
		if agency.idAgency >= AgencyID:
			AgencyID = agency.idAgency +1
	db.insert('Agency', idAgency=AgencyID, telephone=tele,locationAgency=location)

def get_agencies():
	return db.select('Agency', order='idAgency DESC')
def update_agency(agid, tel, loca):
	db.update('Agency', where='idAgency=$agid', telephone=tel, locationAgency=loca, vars=locals())
def get_agency(agid):
	try: 
		return db.select('Agency', where='idAgency=$agid', vars=locals())[0]
	except IndexError:
		return None

def get_agencyID(houseID):
	return db.select('Agency_has_House', where="idHouse=$houseID", vars=locals())[0]
def get_aghashouse(agid):
	return db.select('Agency_has_House', where='Agency_idAgency=$agid', vars=locals())

def del_agency(agid):
	try:
		db.delete('Agency', where="idAgency=$agid", vars=locals())
		return 1
	except Exception:
		return 0
def del_aghashouse(hsid):
	try:
		db.delete('Agency_has_House', where="idHouse=$hsid", vars=locals())
	except Exception:
		return None


def new_client(Name, Password, Email, Credit_Card_No, Phone, Gender):
	clients = get_clients()
	Clientid = 0
	for client in clients:
		if client.idClient >= Clientid:
			Clientid = client.idClient + 1
	db.insert('Client', username=Name, email=Email, password=Password, 
    	created_time=datetime.datetime.utcnow(), credit_no=Credit_Card_No, 
    	gender=Gender, idClient=Clientid)
def del_client(id):
    db.delete('Client', where="idClient=$id", vars=locals())

def update_client(id, Name, Password, Email, Credit_Card_No, Phone, Gender):
    del_client(id)
    db.insert('Client', username=Name, email=Email, password=Password, 
    	created_time=datetime.datetime.utcnow(), credit_no=Credit_Card_No, 
    	gender=Gender, idClient=id)
def get_clients():
    return db.select('Client', order='idClient DESC')

def get_client(id):
    try:
        return db.select('Client', where='idClient=$id', vars=locals())[0]
    except IndexError:
        return None

def visit_exsit(name):
	try:
		db.select('Visit', where="Client_username=$name", vars=locals())[0]
		return 1
	except IndexError:
		return 0

def new_visit(name, date, houseID, loca, agid):
	db.insert('Visit', Client_username=name, Agency_idAgency=agid, House_idHouse=houseID, dateVisit=date, locationHouse=loca)

def get_visitor(username):
    try:
        return db.select('Visit', where='Client_username=$username', vars=locals())[0]
    except IndexError:
        return None
def get_visitors():
	return db.select('Visit', order='House_idHouse DESC')

def update_visitor(oldname, newname, olddate, newdate, loca, houseID, agid):
    del_visitor(oldname, olddate)
    new_visit(newname, newdate, houseID, loca, agid)

def del_visitor(name, date):
    db.delete('Visit', where="Client_username=$name and dateVisit=$date", vars=locals())


def get_house(keyID):
	return db.select('House', where='idHouse=$keyID', vars=locals())[0]
def get_houses():
	return db.select('Agency_has_House', order='idHouse')

def house_book(name, hsid, length, cost):
	db.update('House', where='idHouse=$hsid', ordered_status=1, vars=locals())
	print length
	db.insert('Client_rents_House', Client_username=name, idHouse=hsid, duration=length, startDate=datetime.datetime.utcnow(), cost=cost)

def hs_booked(name, hsid):
	try:
		db.select('Client_rents_House', where="Client_username=$name AND idHouse=$hsid", vars=locals())[0]
		return 1
	except IndexError:
		return 0	

def get_order(houseID):
	try:
		return db.select('Client_rents_House', where="idHouse=$houseID", vars=locals())[0]
	except IndexError:
		return None
def order_delete(hsid):
	db.update('House', where='idHouse=$hsid', ordered_status=0, vars=locals())
	db.delete('Client_rents_House', where="idHouse=$hsid", vars=locals())

def order_update(name, hsid, length):
	order_delete(hsid)
	house_book(name, hsid, length)

def agencywantshouse(agid, hsid):
	try:
		db.insert('Agency_has_House', Agency_idAgency = agid, idHouse = hsid)
	except Exception:
		return None

def get_Allhouses():
	return db.select('House', order='idHouse')
def new_house(price, type, year, loca, capacity):
	houses = get_Allhouses()
	hsid = 0
	for house in houses:
		if house.idHouse >= hsid:
			hsid = house.idHouse+1
	db.insert('House', idHouse=hsid, price=price, ordered_status = 0, type=type, year=year,locationHouse=loca, cap = capacity)
def update_house(houseid, price, type, year, loca, capacity):
	db.update('House', where="idHouse=$houseid", price=price, type=type, year =year, locationHouse=loca, cap = capacity, vars=locals())	
def del_house(hsid):
	try:
		db.delete('House', where="idHouse=$hsid", vars=locals())
	except Exception:
		return None