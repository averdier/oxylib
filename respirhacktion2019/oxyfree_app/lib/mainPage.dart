import 'package:flutter/material.dart';
import 'package:material_switch/material_switch.dart';
import 'Settings.dart';

class MainPage extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _MainPageState();
  }
}

class _MainPageState extends State<MainPage> {
  String selectedSwitchOption = "Auto";
  List<String> switchOptions = ["Auto", "Manuel"];
  var debit = 0;

  changeDebit(value) {
    this.setState(() {
      debit += value;
    });
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
        actions: <Widget>[
          FlatButton(child: Icon(Icons.settings), onPressed: () {
            Navigator.push(context, MaterialPageRoute(builder: (context) => Settings()));
          },)
        ],
      ),
      body: Column(children: <Widget>[
        MaterialSwitch(
          padding: const EdgeInsets.only(left: 65.0, top: 10.0, bottom: 10.0),
          margin: const EdgeInsets.all(5.0),
          selectedOption: selectedSwitchOption,
          options: switchOptions,
          selectedBackgroundColor: Colors.blueGrey,
          selectedTextColor: Colors.white,
          onSelect: (String selectedOption) {
            setState(() {
              selectedSwitchOption = selectedOption;
            });
          },
        ),
        FittedBox(
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
        ),
        FittedBox(
          child: Container(
            child: Card(
              child: Container(
                  width: 500,
                  height: 500,
                  child: Column(
                    children: <Widget>[
                      Container(
                        child: Text(
                          "Débit actuel",
                          style: TextStyle(
                            fontSize: 25,
                          ),
                        ),
                        margin: const EdgeInsets.all(10),
                      ),
                      FittedBox(
                        child: Container(
                          child: Container(
                            child: Column(
                              children: <Widget>[
                                Text(
                                  debit.toString(),
                                  style: TextStyle(
                                      fontSize: 70, color: Colors.white),
                                ),
                                Text(
                                  "mL",
                                  style: TextStyle(
                                      fontSize: 35, color: Colors.white),
                                ),
                              ],
                            ),
                            padding: const EdgeInsets.all(80),
                          ),
                          width: 300,
                          height: 300,
                          decoration: BoxDecoration(
                              shape: BoxShape.circle, color: Colors.blueGrey),
                        ),
                        fit: BoxFit.fill,
                      ),
                      FittedBox(
                        child: Row(
                          children: <Widget>[
                            FlatButton(
                              child: Container(child: Icon(Icons.keyboard_arrow_up, size: 50,), padding: const EdgeInsets.all(20), margin: const EdgeInsets.all(25),),
                              onPressed: () {
                                changeDebit(1);
                              },
                            ),
                            FlatButton(
                              child: Container(child: Icon(Icons.keyboard_arrow_down, size: 50,), padding: const EdgeInsets.all(20), margin: const EdgeInsets.all(25),),
                              onPressed: () {
                                changeDebit(-1);
                              },
                            ),
                          ],
                        ),
                        fit: BoxFit.fill,
                      )
                    ],
                  )),
            ),
            margin: const EdgeInsets.all(10),
          ),
          fit: BoxFit.fill,
        )
      ]),
    );
  }
}
