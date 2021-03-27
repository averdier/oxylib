import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'Bluetooth.dart';
import 'CurrentDebit.dart';
import 'Infos.dart';
import 'Switch.dart';

class MainPage extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _MainPageState();
  }
}

class _MainPageState extends State<MainPage> {
  FlutterBluetoothSerial bluetooth = FlutterBluetoothSerial.instance;
  List<BluetoothDevice> devices = [];
  var device;
  var debit = 0;
  var blue;
  bool connected = false;
  var color = Colors.black;

  _MainPageState() {
    getDevices().then((ok) {
      this.blue = Bluetooth(this.device, setConnectStatus);
    });
  }

  getDevices() async {
    devices = await bluetooth.getBondedDevices();
    for (var i in devices) {
      if (i.name == "HC-05") {
        this.device = i;
      }
    }
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
          FlatButton(
            child: Icon(Icons.settings_bluetooth, color: this.color,),
            onPressed: () {
              if (!this.connected)
                this.blue = Bluetooth(this.device, setConnectStatus);
            },
          ),
        ],
      ),
      body: Column(children: <Widget>[
        SwitchAuto(),
        Infos(),
        CurrentDebit(this.update, this.getConnectStatus),
      ]),
    );
  }

  update(debit) {
      if (this.debit > debit) {
        this.blue.sendMessage("d");
        print("d");
      } else {
        this.blue.sendMessage("i");
        print("i");
      }
      this.setState(() {
        this.debit = debit;
      });
  }

  setConnectStatus(connected) {
    print(connected);
    this.setState(() {
      this.connected = connected;
      if (connected)
        this.color = Colors.blueGrey;
      else
        this.color = Colors.black;
    });
  }

  getConnectStatus() {
    return this.connected;
  }
}
