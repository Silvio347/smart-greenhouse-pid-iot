import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_charts/charts.dart';
import '../models/chart_data.dart';

Widget buildGraph(
    List<ChartData> humidityChartData, List<ChartData> setpointChartData) {
  return SfCartesianChart(
    primaryXAxis: DateTimeAxis(
      // Usar DateTime no eixo X
      majorGridLines: MajorGridLines(width: 1),
      labelStyle: TextStyle(color: Colors.white),
      title: AxisTitle(
        text: 'Time',
        textStyle: TextStyle(color: Colors.white, fontSize: 14),
      ),
      minimum: humidityChartData.isNotEmpty
          ? humidityChartData.first.time
          : DateTime.now().subtract(Duration(minutes: 120)), // Histórico de 2 horas
      maximum: humidityChartData.isNotEmpty
          ? humidityChartData.last.time.add(Duration(seconds: 5))
          : DateTime.now(),
    ),
    primaryYAxis: NumericAxis(
      axisLine: AxisLine(width: 0),
      majorTickLines: MajorTickLines(width: 1),
      labelStyle: TextStyle(color: Colors.white),
      title: AxisTitle(
        text: 'Humidity (%)',
        textStyle: TextStyle(color: Colors.white, fontSize: 14),
      ),
    ),
    legend: Legend(
      isVisible: true,
      position: LegendPosition.top,
      textStyle: TextStyle(color: Colors.white),
    ),
    tooltipBehavior: TooltipBehavior(enable: true),
    zoomPanBehavior: ZoomPanBehavior(
      enableDoubleTapZooming: true,
      enablePanning: true,
      enablePinching: true,
      zoomMode: ZoomMode.x,
    ),
    series: <ChartSeries>[
      LineSeries<ChartData, DateTime>(
        name: 'Humidity',
        dataSource: humidityChartData,
        xValueMapper: (ChartData data, _) => data.time,
        yValueMapper: (ChartData data, _) => data.value,
        color: Colors.black,
        width: 2,
      ),
      LineSeries<ChartData, DateTime>(
        name: 'Setpoint',
        dataSource: setpointChartData,
        xValueMapper: (ChartData data, _) => data.time,
        yValueMapper: (ChartData data, _) => data.value,
        color: Colors.orange,
        width: 2,
        dashArray: <double>[5, 20],
      ),
    ],
  );
}
