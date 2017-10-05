<!DOCTYPE html>
<html lang="en">

    <head>

        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="description" content="">
        <meta name="author" content="">

        <title>Emergiendo con el sol - Página principal</title>

        <!-- Bootstrap Core CSS -->
        <link href="../bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">

        <!-- Bootstrap Core CSS -->
        <link rel="stylesheet" type="text/css" href="../bower_components/datapicker/jquery.ui.datepicker-style.css">

        <!-- MetisMenu CSS -->
        <link href="../bower_components/metisMenu/dist/metisMenu.min.css" rel="stylesheet">

        <!-- Timeline CSS -->
        <link href="../dist/css/timeline.css" rel="stylesheet">

        <!-- Custom CSS -->
        <link href="../dist/css/sb-admin-2.css" rel="stylesheet">

        <!-- Morris Charts CSS -->
        <link href="../bower_components/morrisjs/morris.css" rel="stylesheet">

        <!-- Custom Fonts -->
        <link href="../bower_components/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">

        <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
        <!--[if lt IE 9]>
            <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
            <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
        <![endif]-->

    </head>

    <body>

        <div id="wrapper">

            <!-- Navigation -->
            <nav class="navbar navbar-default navbar-static-top" role="navigation" style="margin-bottom: 0">
                <div class="navbar-header">
                    <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                        <span class="sr-only">Toggle navigation</span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </button>
                    <a class="navbar-brand" href="main.php">Emergiendo con el sol</a>
                </div>
                <!-- /.navbar-header -->

                <ul class="nav navbar-top-links navbar-right">


                    <li class="dropdown">
                        <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                            <i class="fa fa-user fa-fw"></i>  <i class="fa fa-caret-down"></i>
                        </a>
                        <ul class="dropdown-menu dropdown-user">
                            <li><a href="#"><i class="fa fa-user fa-fw"></i> User Profile</a>
                            </li>
                            <li><a href="#"><i class="fa fa-gear fa-fw"></i> Settings</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="login.html"><i class="fa fa-sign-out fa-fw"></i> Logout</a>
                            </li>
                        </ul>
                        <!-- /.dropdown-user -->
                    </li>
                    <!-- /.dropdown -->
                </ul>
                <!-- /.navbar-top-links -->

                <div class="navbar-default sidebar" role="navigation">
                    <div class="sidebar-nav navbar-collapse">
                        <ul class="nav" id="side-menu">
                            <li class="active">
                                <a href="#"><i class="fa fa-bar-chart-o fa-fw"></i> Centrales<span class="fa arrow"></span></a>
                                <ul class="nav nav-second-level">
                                    <?php
                                    $central = array();
                                    $central[0] = array("Lima", "01_Lima");
                                    $central[1] = array("Arequipa", "02_Arequipa");
                                    $central[2] = array("Tacna", "03_Tacna");
                                    //echo "<script>console.log('" . $central[0][0]  . "');</script>";
                                    for ($i = 0; $i < sizeof($central); $i++) {
                                        echo "<li> <a class=\"fetchSeries\" id=\"" . $central[$i][1] . "\" title=\"Click to see " . $central[$i][0] . " stadistics\"
                                           href=\"PleaseEnableJavascript.html\" onclick=\"
                                                   return false;\">" . $central[$i][0] . "</a></li>";
                                    }
                                    ?>
                                </ul>
                                <!-- /.nav-second-level -->
                            </li>
                        </ul>
                    </div>
                    <!-- /.sidebar-collapse -->
                </div>
                <!-- /.navbar-static-side -->
            </nav>

            <div id="page-wrapper">
                <div class="row">
                    <div class="col-lg-12">
                        <h1 id="currentCentral" class="page-header">Lima</h1>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-12">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <i class="fa"></i>
                                <input id="inCalendar" class="left-align" type="text" readonly="readonly" size="7">
                                <button class="dataUpdate">Actualizar gráfico</button>
                                <div class="pull-right">
                                    <div class="btn-group">
                                        <button type="button" class="btn btn-default btn-xs dropdown-toggle" data-toggle="dropdown">
                                            Actions
                                            <span class="caret"></span>
                                        </button>
                                        <ul class="dropdown-menu pull-right" role="menu">
                                            <li><a href="#">Action</a>
                                            </li>
                                            <li><a href="#">Another action</a>
                                            </li>
                                            <li><a href="#">Something else here</a>
                                            </li>
                                            <li class="divider"></li>
                                            <li><a href="#">Separated link</a>
                                            </li>
                                        </ul>
                                    </div>
                                </div>
                            </div>
                            <div class="panel-body">

                                <div class="demo-container">
                                    <div id="placeholder" class="demo-placeholder" style="float:left; width:85%; height:450px;"></div>
                                    <p id="choices" style="float:right; width:15%;"></p>
                                </div>

                            </div>
                        </div>
                    </div>
                </div>

            </div>
            <!-- /#wrapper -->

            <!-- jQuery -->
            <script src="../bower_components/jquery/dist/jquery.min.js"></script>

            <!-- Bootstrap Core JavaScript -->
            <script src="../bower_components/bootstrap/dist/js/bootstrap.min.js"></script>

            <!-- Metis Menu Plugin JavaScript -->
            <script src="../bower_components/metisMenu/dist/metisMenu.min.js"></script>

            <!-- Flot Charts JavaScript -->
            <script src="../bower_components/flot/jquery.flot.js"></script>
            <script src="../bower_components/flot/jquery.flot.resize.js"></script>
            <script src="../bower_components/flot/jquery.flot.time.js"></script>

            <!-- Data Picker JavaScript -->
            <script src="..\bower_components\datapicker\jquery-ui.min.js"></script>
            <script src="..\\bower_components\datapicker\jquery.ui.datepicker-es.js"></script>

            <!-- Morris Charts JavaScript -->
            <script src="../bower_components/raphael/raphael-min.js"></script>
            <script src="../bower_components/morrisjs/morris.min.js"></script>
            <!--<script src="../dist/js/morris-data.js"></script>-->

            <script type="text/javascript">

                $(function () {
                    $("#inCalendar").datepicker({
                        changeMonth: true,
                        changeYear: true,
                        dateFormat: "dd-mm-yy",
                        maxDate: 0
                    });
                    $("#inCalendar").datepicker('setDate', new Date());

                });

                $(function () {

                    /*var data = [];
                     
                     var options = {
                     lines: {
                     show: true
                     },
                     points: {
                     show: true
                     },
                     xaxis: {
                     tickDecimals: 0,
                     tickSize: 1
                     }
                     };*/

                    //$.plot("#placeholder", data, options);

                    // Fetch one series, adding to what we already have

                    var centralID;

                    $("a.fetchSeries").click(function () {

                        var ref = $(this);
                        var temp = $("#inCalendar").datepicker('getDate');
                        centralID = ref.attr("id");
                        $('#currentCentral').html(ref.html());
                        refreshChart(temp, centralID);
                        //console.log(temp.valueOf() + " " + centralID);
                        //var dataurl = button.siblings("a").attr("href");

                    });

                    $("button.dataUpdate").click(function () {

                        var temp = $("#inCalendar").datepicker('getDate');
                        //console.log(temp.valueOf() + " " + centralID);
                        refreshChart(temp, centralID);

                    });

                    function refreshChart(date, central) {
                        date = new Date(date);
                        date = ((date.getTime() /1000) - date.getTimezoneOffset() * 60);
                        console.log("First date: " + date);
                        
                        $.ajax({
                            url: 'mysqlphp.php',
                            type: 'POST',
                            data: {var1: date, var2: central},
                            success: function (data) {
                                var datasets = JSON.parse(data);
                                var i = 0;
                                $.each(datasets, function (key, val) {
                                    val.color = i;
                                    ++i;
                                });

                                // insert checkboxes 
                                var choiceContainer = $("#choices");
                                choiceContainer.empty();
                                $.each(datasets, function (key, val) {
                                    choiceContainer.append("<br/><input type='checkbox' name='" + key +
                                            "' checked='checked' id='id" + key + "'></input>" +
                                            "<label for='id" + key + "'>"
                                            + val.label + "</label>");
                                });

                                choiceContainer.find("input").click(plotAccordingToChoices);

                                function plotAccordingToChoices() {

                                    var data = [];

                                    choiceContainer.find("input:checked").each(function () {
                                        var key = $(this).attr("name");
                                        if (key && datasets[key]) {
                                            data.push(datasets[key]);
                                        }
                                    });
                                    
                                    console.log("Dato:" + new Date(date * 1000).toUTCString());
                                    console.log("Dato:" + new Date((date + 86400)*1000).toUTCString());
                                    
                                    console.log("Dato:" + new Date(date * 1000));
                                    console.log("Dato:" + new Date((date + 86400)*1000));

                                    if (data.length > 0) {
                                        $.plot("#placeholder", data, {
                                            yaxis: {
                                                // min: 0
                                            },
                                            xaxis: {
                                                mode: "time",
                                                timeformat: "%h",
                                                tickSize: [1, "hour"],
                                                //color: "black",
                                                axisLabel: "Horas",
                                                axisLabelUseCanvas: true,
                                                axisLabelFontSizePixels: 12,
                                                axisLabelFontFamily: 'Verdana, Arial',
                                                axisLabelPadding: 10,
                                                min: (date) * 1000,
                                                max: (date + 86400) * 1000
                                            }
                                            /*xaxis: {
                                             mode: "time",
                                             timeformat: "%I:%M %p", // HH:MM am/pm
                                             tickSize: [1, "hour"], // tick every hour
                                             twelveHourClock: true
                                             
                                             }*/
                                        });
                                        //$("#placeholder").UseTooltip();
                                    }
                                }

                                plotAccordingToChoices();
                            }
                        });
                    }



                    // Load the first series by default, so we don't have an empty plot

                    $("a.fetchSeries:first").click();

                });

            </script>



            <!-- Custom Theme JavaScript -->F
            <script src="../dist/js/sb-admin-2.js"></script>

            <script>
                /*function changeCentral(central) {
                 console.log("Esto es " + central);
                 }*/
            </script>

    </body>

</html>
