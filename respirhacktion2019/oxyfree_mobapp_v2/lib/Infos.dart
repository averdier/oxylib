import 'package:flutter/material.dart';

class Infos extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _InfosState();
  }
}

class _InfosState extends State<Infos> {
  @override
  Widget build(BuildContext context) {
    return FittedBox(
      child: Container(
        child: Row(
          children: <Widget>[
            Card(
                child: Container(
                  child: Column(
                    children: <Widget>[
                      Text("Sat. Oxygène"),
                      Container(
                        child: Text(
                          "94",
                          style: TextStyle(
                            fontSize: 30,
                          ),
                        ),
                        padding: const EdgeInsets.all(20),
                      ),
                      Text(
                        "%",
                        style: TextStyle(fontSize: 15),
                      ),
                    ],
                  ),
                  padding: EdgeInsets.all(10.0),
                  width: 150,
                )),
            Card(
                child: Container(
                  child: Column(
                    children: <Widget>[
                      Text("Rythme Cardiaque"),
                      Container(
                        child: Text(
                          "85",
                          style: TextStyle(
                            fontSize: 30,
                          ),
                        ),
                        padding: const EdgeInsets.all(20),
                      ),
                      Text(
                        "BPM",
                        style: TextStyle(fontSize: 15),
                      ),
                    ],
                  ),
                  width: 150,
                  padding: EdgeInsets.all(10.0),
                )),
            Card(
                child: Container(
                  child: Column(
                    children: <Widget>[
                      Text("Débit Idéal"),
                      Container(
                        child: Text(
                          "3.4",
                          style: TextStyle(
                            fontSize: 30,
                          ),
                        ),
                        padding: const EdgeInsets.all(20),
                      ),
                      Text(
                        "mL",
                        style: TextStyle(fontSize: 15),
                      ),
                    ],
                  ),
                  width: 150,
                  padding: EdgeInsets.all(10.0),
                )),
          ],
        ),
        margin: EdgeInsets.all(10),
      ),
      fit: BoxFit.fill,
    );
  }
}