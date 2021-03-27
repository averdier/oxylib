import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'dart:convert';
import 'dart:async';
import 'dart:typed_data';

class Settings extends StatefulWidget {
  @override
  _SettingsState createState() => new _SettingsState();
}

class _SettingsState extends State<Settings> {
  BluetoothConnection connection;
  bool get isConnected => connection != null && connection.isConnected;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Text("settings"),
    );
  }
}