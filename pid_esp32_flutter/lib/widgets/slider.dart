import 'package:flutter/material.dart';

class CustomSlider extends StatefulWidget {
  final String initialValue;
  final String minValue;
  final String maxValue;
  final Function(double) onChanged;
  final String labelText;

  CustomSlider({
    required this.initialValue,
    required this.minValue,
    required this.maxValue,
    required this.onChanged,
    required this.labelText,
  });

  @override
  _CustomSliderState createState() => _CustomSliderState();
}

class _CustomSliderState extends State<CustomSlider> {
  late double _currentValue;

  @override
  void initState() {
    super.initState();
    _currentValue = double.tryParse(widget.initialValue) ??
        0.0; 
  }

  @override
  Widget build(BuildContext context) {
    double parsedMinValue = double.tryParse(widget.minValue) ?? 0.0;
    double parsedMaxValue = double.tryParse(widget.maxValue) ?? 0.0;

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          widget.labelText,
          style: TextStyle(
            color: Colors.white,
            fontSize: 18,
            fontWeight: FontWeight.bold,
          ),
        ),
        Slider(
          value: _currentValue,
          activeColor: Colors.white,
          thumbColor: Colors.white,
          inactiveColor: Colors.white,
          min: parsedMinValue,
          max: parsedMaxValue,
          divisions: (parsedMaxValue - parsedMinValue).toInt(),
          label:
              _currentValue.toStringAsFixed(0),
          onChanged: (value) {
            setState(() {
              _currentValue = value;
            });
            widget.onChanged(value);
          },
        ),
        Center(
          child: Text(
            '${_currentValue.toStringAsFixed(0)}%',
            style: TextStyle(color: Colors.white, fontSize: 14),
          ),
        ),
      ],
    );
  }
}
