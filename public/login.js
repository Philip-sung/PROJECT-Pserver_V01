function login() {
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    if (username == "username" && password == "password") {
        alert("Login successful");
        return false;
    } else {
        alert("Login failed");
    }
}