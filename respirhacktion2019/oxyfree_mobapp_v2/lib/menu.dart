import 'package:flutter/material.dart';
import 'mainPage.dart';
import 'geoloc.dart';

class Menu extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _MenuState();
  }
}

class _MenuState extends State<Menu> {

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
      body: Container(
        child: Column(
          children: <Widget>[
            Row(
              mainAxisSize: MainAxisSize.max,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Padding(
                  padding: EdgeInsets.all(20.0),
                  child: RaisedButton(
                    padding: EdgeInsets.all(10.0),
                    color: Colors.white,
                    child: Column(
                      children: <Widget>[
                        Icon(Icons.favorite, size: 100.0, color: Colors.red),
                        Text("Régulateur")
                      ],
                    ),
                    onPressed: (){
                      Navigator.of(context).push(MaterialPageRoute(builder: (_) {
                        return MainPage();
                      }));
                    },
                  )
                ),
                Padding(
                  padding: EdgeInsets.all(20.0),
                  child: RaisedButton(
                    padding: EdgeInsets.all(10.0),
                    color: Colors.white,
                    child: Column(
                      children: <Widget>[
                        Icon(Icons.my_location, size: 100.0),
                        Text("Géolocalisation")
                      ],
                    ),
                    onPressed: (){
                      Navigator.of(context).push(MaterialPageRoute(builder: (_) {
                        return Geoloc();
                      }));
                    },
                  )
                ),
              ],
            ),
            /*Row(
              mainAxisSize: MainAxisSize.max,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Padding(
                  padding: EdgeInsets.all(20.0),
                  child: RaisedButton(
                    padding: EdgeInsets.all(10.0),
                    color: Colors.white,
                    child: Column(
                      children: <Widget>[
                        Icon(Icons.favorite, size: 100.0, color: Colors.red),
                        Text("Régulateur")
                      ],
                    ),
                    onPressed: (){
                      Navigator.of(context).push(MaterialPageRoute(builder: (_) {
                        return MainPage();
                      }));
                    },
                  )
                ),
                Padding(
                  padding: EdgeInsets.all(20.0),
                  child: RaisedButton(
                    padding: EdgeInsets.all(10.0),
                    color: Colors.white,
                    child: Column(
                      children: <Widget>[
                        Icon(Icons.cloud),
                        Text("Météo")
                      ],
                    ),
                    onPressed: (){
                    },
                  )
                ),
              ],
            ),*/
          ],
        ),
      )
    );
  }
}