function build(db, ds){
	$.get("http://www.lvxiaoxin.com:8080/build/" + db + "/" + ds, function(data, status){
		if(status=="success"){
			alert(data);
		}
	});
}

function load(db) {
	$.get("http://www.lvxiaoxin.com:8080/load/" + db, function(data, status){
		if(status=="success"){
			alert(data);
		}
	});
}

function query(dp) {
	$.get("http://www.lvxiaoxin.com:8080/query/" + dp, function(data, status){
		if(status=="success"){
			alert(data);
		}
	});
}

function unload(db) {
	$.get("http://www.lvxiaoxin.com:8080/unload", function(data, status){
		if(status=="success"){
			alert(data);
		}
	});
}
