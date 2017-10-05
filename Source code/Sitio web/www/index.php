<?php
/* * * begin our session ** */
session_start();


if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $valid = false;
    /*     * * check if the users is already logged in ** */
    if (isset($_SESSION['user_id'])) {
        $message = 'Users is already logged in.';
    }
    /*     * * check that both the username, password have been submitted ** */
    if (!isset($_POST['user_username'], $_POST['user_password'])) {
        $message = 'Please enter a valid username and password.';
    }
    /*     * * check the username is the correct length ** */ elseif (strlen($_POST['user_username']) > 20 || strlen($_POST['user_username']) < 4) {
        $message = 'Incorrect Length for Username.';
    }
    /*     * * check the password is the correct length ** */ elseif (strlen($_POST['user_password']) > 20 || strlen($_POST['user_password']) < 4) {
        $message = 'Incorrect Length for Password.';
    }
    /*     * * check the username has only alpha numeric characters ** */ elseif (ctype_alnum($_POST['user_username']) != true) {
        /*         * * if there is no match ** */
        $message = "Username must be alpha numeric.";
    }
    /*     * * check the password has only alpha numeric characters ** */ elseif (ctype_alnum($_POST['user_password']) != true) {
        /*         * * if there is no match ** */
        $message = "Password must be alpha numeric.";
        
    } else {
        echo 'He entrado';
        
        /*         * * if we are here the data is valid and we can insert it into database ** */
        $user_username = filter_var($_POST['user_username'], FILTER_SANITIZE_STRING);
        $user_password = filter_var($_POST['user_password'], FILTER_SANITIZE_STRING);

        /*         * * now we can encrypt the password ** */
        $user_password = sha1($user_password);



        /*         * * connect to database ** */
        /*         * * mysql hostname ** */


        try {

            require './mysqlData.php';
            $dbh = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
            /*             * * $message = a message saying we have connected ** */

            /*             * * set the error mode to excptions ** */
            $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            /*             * * prepare the select statement ** */
            $stmt = $dbh->prepare("SELECT id, username, password FROM users 
                    WHERE username = :username AND password = :password");

            /*             * * bind the parameters ** */
            $stmt->bindParam(':username', $user_username, PDO::PARAM_STR);
            $stmt->bindParam(':password', $user_password, PDO::PARAM_STR, 40);



            /*             * * execute the prepared statement ** */
            $stmt->execute();

            /*             * * check for a result ** */
            $user_id = $stmt->fetchColumn();

            /*             * * if we have no result then fail boat ** */
            
            

            if ($user_id == false) {
                $message = 'Login Failed.';
            }
            /*             * * if we do have a result, all is well ** */ else {
                /*                 * * set the session user_id variable ** */
                $_SESSION['user_id'] = $user_id;
//$_SESSION['user_name'] = $user_name;

                /*                 * * tell the user we are logged in ** */
                $valid = true;
                header('Location: main.php');
                $message = 'You are now logged in.';
            }
        } catch (Exception $e) {
            /*             * * if we are here, something has gone wrong with the database ** */
            $message = 'We are unable to process your request. Please try again later.';
        }
    }
}
?>


<!DOCTYPE html>
<html lang="en">

    <head>

        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="description" content="">
        <meta name="author" content="">

        <title>SB Admin 2 - Bootstrap Admin Theme</title>

        <!-- Bootstrap Core CSS -->
        <link href="../bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">

        <!-- MetisMenu CSS -->
        <link href="../bower_components/metisMenu/dist/metisMenu.min.css" rel="stylesheet">

        <!-- Custom CSS -->
        <link href="../dist/css/sb-admin-2.css" rel="stylesheet">
        <link href="../CSS/basic.css" rel="stylesheet">

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

        <div class="container">
            <div class="row">
                <div class="blank" class="col-lg-4 col-md-4 col-sm-4 col-xs-0">
                </div>
                <div class="col-md-6 col-md-offset-3">
                    <?php if (($_SERVER["REQUEST_METHOD"] == "POST") && ($valid === false)): ?>
                        <div class="alert alert-dismissible alert-danger">
                            <button type="button" class="close" data-dismiss="alert">×</button>
                            <strong>Oh snap!</strong> <?php echo $message; ?> <a href="#" class="alert-link">Change it</a> and try submitting again.

                        </div>
                    <?php endif; ?>
                </div>
            <!--</div>
            <div class="row">-->
                <div class="col-md-4 col-md-offset-4">
                    <div class="login-panel panel panel-default">
                        <div class="panel-heading">
                            <h3 class="panel-title">Login</h3>
                        </div>
                        <div class="panel-body">
                            <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
                                <fieldset>
                                    <div class="form-group">
                                        <input class="form-control" placeholder="Usuario" name="user_username" type="text" autofocus>
                                    </div>
                                    <div class="form-group">
                                        <input class="form-control" placeholder="Contraseña" name="user_password" type="password" value="">
                                    </div>
                                    <!--<div class="checkbox">
                                        <label>
                                            <input name="remember" type="checkbox" value="Remember Me">Remember Me
                                        </label>
                                    </div>-->
                                    <!-- Change this to a button or input when using this as a form -->
                                    <input type="submit" value="Entrar" />
                                </fieldset>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- jQuery -->
        <script src="../bower_components/jquery/dist/jquery.min.js"></script>

        <!-- Bootstrap Core JavaScript -->
        <script src="../bower_components/bootstrap/dist/js/bootstrap.min.js"></script>

        <!-- Metis Menu Plugin JavaScript -->
        <script src="../bower_components/metisMenu/dist/metisMenu.min.js"></script>

        <!-- Custom Theme JavaScript -->
        <script src="../dist/js/sb-admin-2.js"></script>

    </body>

</html>
