function Submit() {
    var titleData = retrieveData()
    console.log(titleData);
}

function retrieveData() {
    var name1 = document.getElementById("title").value
    var myTextarea = document.getElementById("myTextarea").value

    var arr = [name1, myTextarea];
    return arr;
}