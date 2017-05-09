function bLogin() {
  var i_name = document.getElementById('username1');
  var i_password = document.getElementById('password1');

  var name = i_name.value;
  var password = i_password.value;

  myJSON = JSON.stringify({
    "key": "login",
    "name": name,
    "password": password,
  });
  sendMessage(myJSON);
}


function bSignup() {
  var i_name = document.getElementById('username2');
  var i_password = document.getElementById('rpassword2');
  var i_email = document.getElementById('email2');

  var name = i_name.value;
  var password = i_password.value;
  var email = i_email.value;

  myJSON = JSON.stringify({
    "key": "signup",
    "name": name,
    "password": password,
    "email": email,
  });
  sendMessage(myJSON);
}


function sMode() {
  
}