#!/usr/bin/python3
from pickle import TRUE
from flask import Flask, request
from flask_restx import Resource, Api, fields, reqparse
from datetime import date
import logging
import mysql.connector
import json

today = date.today()

d1 = today.strftime("%d%m%Y")

logging.basicConfig(filename='./logs/logs_'+d1+'.log', level=logging.INFO, format=f'%(asctime)s %(levelname)s : %(message)s')

config = json.load(open("./configs.json"))

mydb = mysql.connector.connect(
  host= config["host"],
  user= config["user"],
  password= config["password"],
  database= config["database"]
) 

mycursor = mydb.cursor()

app = Flask(__name__)

api = Api(app=app, version="0.1", doc="/api", title="API station météo", description="API dédié au projet IOT cube", default="API", default_label='', validate=True)

sensors_post_definition = api.model('sensors post informations', {
    'latitude': fields.String(required=True),
    'longitude': fields.String(required=True),
    'active': fields.Boolean(required=True)
})

sensors_put_definition = api.model('sensors put informations', {
    'latitude': fields.String(),
    'longitude': fields.String(),
    'active': fields.Boolean()
})

@api.route("/api/v1/sensors/<id>")
class sonde(Resource):
    @api.response(200, 'API sensors : Success')
    def get(self, id):
        """
        Recherche d'un élément de la table id
        """
        if id:
          mycursor.execute("SELECT * FROM sensors WHERE `sensorID` = '" + id + "';")
          myresult = mycursor.fetchall()
          print(myresult)
          jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
          return json.loads(jso), 200
        else:
          return {"error" : "missing arguments."}, 400 
    @api.expect(sensors_put_definition)
    def put(self, id):
      """
      Update dans la table sensors
      """
      if id:
        request_data = request.get_json()
        at_change = ""
        for element in request_data:
          if element == "latitude" or element == "longitude" or element == "active":
            if at_change == "":
              at_change = at_change + element + "='" + request_data[element] + "'"
            else:
              at_change = at_change + "," + element + "='" + request_data[element] + "'" 
            
        if at_change != "":
          mycursor.execute("UPDATE sensors SET " + at_change + " WHERE sensorID = " + id +";")
          mydb.commit()
          return {"statut":"success"}, 200
      else:
        return {"error" : "missing arguments."}, 400
    def delete(self, id):
      """
      Update dans la table sensors
      """
      if id:
        mycursor.execute("DELETE FROM `sensors` WHERE sensorID = " + id +";")
        mydb.commit()
        return {"statut":"success"}, 200
      else:
        return {"error" : "missing arguments."}, 400

@api.route("/api/v1/sensors")
class sonde(Resource):
    @api.response(200, 'API sensors : Success')
    def get(self):
        """
        Récupération de la table sensors
        """
        mycursor.execute("SELECT * FROM sensors")
        myresult = mycursor.fetchall()

        jso = json.dumps(myresult, indent=4, sort_keys=True, default=str)
        return json.loads(jso), 200

    @api.expect(sensors_post_definition)
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
            mycursor.execute("INSERT INTO `sensors` (`latitude`, `longitude`, `active`) VALUES ('"+ str(request_data["latitude"])+"', '"+str(request_data["longitude"])+"', '"+str(request_data["active"]) + "');")
            mydb.commit()
            return {"statut":"success"}, 200
        else:
          return {"error" : "missing arguments."}, 400

if __name__ == '__main__':
    app.run(host='192.168.95.217', port='5010', debug=True)