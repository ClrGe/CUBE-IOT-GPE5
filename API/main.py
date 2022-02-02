#!/usr/bin/python3
from pickle import TRUE
from this import d
from flask import Flask, request, render_template
from flask_restx import Resource, Api, fields, reqparse
from datetime import date
import logging
import mysql.connector
import json

today = date.today()

d1 = today.strftime("%d%m%Y")

logging.basicConfig(filename='./logs/logs_'+d1+'.log', level=logging.INFO,
                    format=f'%(asctime)s %(levelname)s : %(message)s')

config = json.load(open("./configs.json"))

mydb = mysql.connector.connect(
    host=config["host"],
    user=config["user"],
    password=config["password"],
    database=config["database"]
)

mycursor = mydb.cursor()

app = Flask(__name__)

api = Api(app=app, version="0.1", doc="/api", title="API station météo",
          description="API dédié au projet IOT cube", default="API", default_label='', validate=True)

sensor = api.namespace("api/v1/sensors", description="CRUD de la table sensors")

dataa = api.namespace("api/v1/data", description="CRUD de la table data")

sensors_post_definition = sensor.model('sensors post informations', {
    'latitude': fields.Float(required=True),
    'longitude': fields.Float(required=True),
    'active': fields.Boolean(required=True)
})

data_post_definition = dataa.model('data post informations', {
    'recordDate': fields.DateTime(required=True),
    'sensorID': fields.Integer(required=True),
    'temp': fields.Float(required=True),
    'humidity': fields.Float(required=True),
    'sensor': fields.Integer(required=True)
})

sensors_put_definition = sensor.model('sensors put informations', {
    'latitude': fields.Float(),
    'longitude': fields.Float(),
    'active': fields.Boolean()
})

data_put_definition = dataa.model('data put informations', {
    'recordDate': fields.DateTime(),
    'sensorID': fields.Integer(),
    'temp': fields.Float(),
    'humidity': fields.Float(),
    'sensor': fields.Integer()
})

@app.route("/ui")
def site():
    return render_template('index.html')


@sensor.route("/<id>")
class sonde(Resource):
    @sensor.response(200, 'API sensors : Success')
    def get(self, id):
        """
        Recherche d'une ligne de la table sensors
        """
        if id:
            mycursor.execute(
                "SELECT * FROM sensors WHERE `sensorID` = '" + id + "';")
            myresult = mycursor.fetchall()
            print(myresult)
            jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
            return json.loads(jso), 200
        else:
            return {"error": "missing arguments."}, 400

    @sensor.expect(sensors_put_definition)
    def put(self, id):
        """
        Ajouter d'une ligne dans la table sensors
        """
        if id:
            request_data = request.get_json()
            at_change = ""
            for element in request_data:
                if element == "latitude" or element == "longitude" or element == "active":
                    if at_change == "":
                        at_change = at_change + element + \
                            "='" + str(request_data[element]) + "'"
                    else:
                        at_change = at_change + "," + element + \
                            "='" + str(request_data[element]) + "'"

            if at_change != "":
                mycursor.execute("UPDATE sensors SET " +
                                 at_change + " WHERE sensorID = " + id + ";")
                mydb.commit()
                return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400

    def delete(self, id):
        """
        Suppression d'une ligne dans la table sensors
        """
        if id:
            mycursor.execute(
                "DELETE FROM `sensors` WHERE sensorID = " + id + ";")
            mydb.commit()
            return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400


@sensor.route("")
class sonde(Resource):
    @sensor.response(200, 'API sensors : Success')
    def get(self):
        """
        Récupération de la table sensors
        """
        mycursor.execute("SELECT * FROM sensors")
        myresult = mycursor.fetchall()

        jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
        return json.loads(jso), 200

    @sensor.expect(sensors_post_definition)
    def post(self):
        """
        Ajout dans la table sensors
        """
        request_data = request.get_json()

        if request_data["latitude"] != None and request_data["longitude"] != None and request_data["active"] != None:
            if request_data["active"] == TRUE:
                request_data["active"] = 1
            else:
                request_data["active"] = 0
            mycursor.execute("INSERT INTO `sensors` (`latitude`, `longitude`, `active`) VALUES ('" + str(
                request_data["latitude"])+"', '"+str(request_data["longitude"])+"', '"+str(request_data["active"]) + "');")
            mydb.commit()
            return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400


@dataa.route("")
class data(Resource):
    @dataa.response(200, 'API data : Success')
    def get(self):
        """
        Récupération de la table data
        """
        mycursor.execute("SELECT * FROM data")
        myresult = mycursor.fetchall()

        jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
        return json.loads(jso), 200

    @dataa.expect(data_post_definition)
    def post(self):
        """
        Ajout dans la table data
        """
        request_data = request.get_json()

        if request_data["recordDate"] != None and request_data["sensorID"] != None and request_data["temp"] != None and request_data["humidity"] != None and request_data["sensor"] != None:
            mycursor.execute("INSERT INTO `data` (`recordDate`, `sensorID`, `temp`, `humidity`, `sensor`) VALUES ('" + str(request_data["recordDate"])+"', '"+str(
                request_data["sensorID"])+"', '"+str(request_data["temp"])+"', '"+str(request_data["humidity"])+"', '"+str(request_data["sensor"]) + "');")
            mydb.commit()
            return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400

@dataa.route("/<id>")
class data(Resource):
    @dataa.response(200, 'API sensors : Success')
    def get(self, id):
        """
        Recherche d'une ligne de la table data
        """
        if id:
            mycursor.execute(
                "SELECT * FROM data WHERE `ID` = '" + id + "';")
            myresult = mycursor.fetchall()
            
            jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
            return json.loads(jso), 200
        else:
            return {"error": "missing arguments."}, 400

    @dataa.expect(data_put_definition)
    def put(self, id):
        """
        Ajouter d'une ligne dans la table data
        """
        if id:
            request_data = request.get_json()
            at_change = ""
            for element in request_data:
                if element == "recordDate" or element == "sensorID" or element == "temp" or element == "humidity" or element == "sensor":
                    if at_change == "":
                        at_change = at_change + element + \
                            "='" + str(request_data[element]) + "'"
                    else:
                        at_change = at_change + "," + element + \
                            "='" + str(request_data[element]) + "'"

            if at_change != "":
                mycursor.execute("UPDATE data SET " +
                                 at_change + " WHERE ID = " + id + ";")
                mydb.commit()
                return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400
    def delete(self, id):
        """
        Suppression d'une ligne dans la table data
        """
        if id:
            mycursor.execute(
                "DELETE FROM `data` WHERE ID = " + id + ";")
            mydb.commit()
            return {"statut": "success"}, 200
        else:
            return {"error": "missing arguments."}, 400

if __name__ == '__main__':
    app.run(host='192.168.18.90', port='5010', debug=True)
