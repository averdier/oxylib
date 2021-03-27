import 'package:flutter/material.dart';
import 'dart:core';
import 'package:geolocator/geolocator.dart';
//import 'package:google_maps_flutter/google_maps_flutter.dart';
//import 'package:location/location.dart';

class Geoloc extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _GeolocState();
  }
}

class _GeolocState extends State<Geoloc> {
  Geolocator geolocator = Geolocator();
  //GoogleMapController mapController;

  //Marker marker;

  Map<String, dynamic> userLocation;

  @override
  void initState() {
    super.initState();
    getCoordonne();
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      appBar: new AppBar(
        backgroundColor: Colors.white,
        leading: Container(
          child: Image.asset(
            'assets/image/logo_oxyfree_simple.png',
          ),
          margin: new EdgeInsets.only(left: 5.0, bottom: 5.0, top: 5.0),
        ),
        title: Text(
          "OxyFree",
          style: TextStyle(color: Colors.black54),
        ),
        /*actions: <Widget>[
          FlatButton(
            child: Icon(Icons.settings_bluetooth),
            onPressed: () {
              if (!this.connected)
                this.blue = Bluetooth(this.device, setConnectStatus);
            },
          ),
        ],*/
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            userLocation == null
                    ? CircularProgressIndicator()
                    : Text("Location: Lat : " +
                        userLocation['latitude'].toString() +
                        ", Long : " +
                        userLocation['longitude'].toString()
                      )
          /*Container(
            height: MediaQuery.of(context).size.height,
            width: MediaQuery.of(context).size.width,
            child: GoogleMap(
              onMapCreated: (GoogleMapController controller) {
                mapController = controller;
              },
              options: GoogleMapOptions(
                cameraPosition: CameraPosition(
                  target: LatLng(37.4219999, -122.0862462),
                ),
                myLocationEnabled: true,
              ),
            ),
          ),*/
        ],
        ),
      ),
    );
  }

  Future<Map> getCoordonne() async {
    geolocator
    .getPositionStream(LocationOptions(
        accuracy: LocationAccuracy.best, timeInterval: 1000))
    .listen((position) async {
        setState(() {
          userLocation = position.toJson();
        });
      print(position.toJson());
            
    });
  }

  /*Future<Position> _getLocation() async {
    var currentLocation;
    try {
      currentLocation = await geolocator.getCurrentPosition(
          desiredAccuracy: LocationAccuracy.best);
    } catch (e) {
      currentLocation = null;
    }
    return currentLocation;
  }*/

}