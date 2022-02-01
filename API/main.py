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

config = json.load(open("./config.json"))

mydb = mysql.connector.connect(
  host= config["host"],
  user= config["user"],
  password= config["password"],
  database= config["database"]
) 

mycursor = mydb.cursor()

app = Flask(__name__)
api = Api(app=app, version="0.1", doc="/api", title="API station météo", description="API dédié au projet IOT cube", default="API", default_label='', validate=True)

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

    def post(self):
        """
        Ajout dans la table sensors
        """
        request_data = request.get_json()

        if request_data["latitude"] != None and request_data["longitude"] != None and request_data["active"] != None:
            mycursor.execute("INSERT INTO `authors` (`latitude`, `longitude`, `active`) VALUES ('"+request_data["latitude"]+"', '"+request_data["longitude"]+"', '"+request_data["active"])
    def put(self):
      """
      Update dans la table sensors
      """
        #return json.loads(jso), 200

if __name__ == '__main__':
    app.run(host='127.0.0.1', port='5010', debug=True)