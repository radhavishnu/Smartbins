from urllib import response
from flask import Flask, request, render_template, url_for
from datetime import datetime
#from influxdb_client import InfluxDBClient, Point, WritePrecision
#from influxdb_client.client.write_api import SYNCHRONOUS

import random

#from graphviz import render


# You can generate an API token from the "API Tokens Tab" in the UI
token = "HemEhFJxMc_AbJ8bGBz2ypDW0Nun0CMsnkWfotQXf_TPUo9cJjCtF8yD1uUNqVF0IjEF6LISOtcMpw3585rg_A=="
org = "AIE24"
bucket = "Dustbin_Admin"

w = 0
app = Flask(__name__)


@app.route("/loc1", methods=["GET"])
def loc1():
    return render_template('loc1.html')


@app.route("/loc2", methods=["GET"])
def loc2():
    return render_template('loc2.html')


@app.route("/loc3", methods=["GET"])
def loc3():
    return render_template('loc3.html')


@app.route("/loc4", methods=["GET"])
def loc4():
    return render_template('loc4.html')


@app.route("/loc5", methods=["GET"])
def loc5():
    return render_template('loc5.html')


@app.route("/loc6", methods=["GET"])
def loc6():
    return render_template('loc6.html')


@app.route("/values", methods=['GET'])
def values():
    with open('database.txt') as f:
        v = (f.readline().split(","))
    return {"value1": v[0], "value2": v[1], "value3": v[2], "value4": v[3]}


@app.route('/setvals', methods=['GET'])
def test():

    print("----------------")
    print(request.args.get("value1"))
    val1 = request.args.get("value1")
    val2 = request.args.get("value2")
    val3 = request.args.get("value3")
    val4 = request.args.get("value4")
    with open('database.txt', 'w') as f:
        f.write(f"{val1},{val2},{val3},{val4}")

    #val2= request.args.get("value2")
    #val3= request.args.get("value3")
    #val4= request.args.get("value4")

    return "200 OK"

    # return w


@app.route('/', methods=['GET'])
def home():
    with open('database.txt') as f:
        v = (f.readline().split(","))
    return render_template('index.html', value1=v[0], value2=v[1], value3=v[2], value4=v[3])


if __name__ == "__main__":

    app.run(host='0.0.0.0', port=5050)
