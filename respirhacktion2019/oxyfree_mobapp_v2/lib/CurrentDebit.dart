import 'package:flutter/material.dart';

class CurrentDebit extends StatefulWidget {
  final update;
  final getConnected;

  CurrentDebit(this.update, this.getConnected);

  @override
  State<StatefulWidget> createState() {
    return _CurrentDebitState(this.update, this.getConnected);
  }
}

class _CurrentDebitState extends State<CurrentDebit> {
  var debit = 0;
  final update;
  final getConnected;

  _CurrentDebitState(this.update, this.getConnected);

  changeDebit(value) {
    this.setState(() {
      if (value == -1 && debit != 0) {
        debit += value;
        update(debit);
      } else if (value == 1 && debit != 6) {
        debit += value;
        update(debit);
      }
    });
  }


  @override
  Widget build(BuildContext context) {
    return FittedBox(
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
                            if (this.getConnected())
                              changeDebit(1);
                          },
                        ),
                        FlatButton(
                          child: Container(child: Icon(Icons.keyboard_arrow_down, size: 50,), padding: const EdgeInsets.all(20), margin: const EdgeInsets.all(25),),
                          onPressed: () {
                            print(this.getConnected());
                            if (this.getConnected())
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
    );
  }
}