
$(function () {
    $(document).on('click', '.plusBtn', function () {
        // zjistime si jeho rodice
        var i = 0;
        var container = $(this).parents('div.custaGroupContainer');
        var row = $(this).parents('div.multipleRow').html();
        i = $(container).children("div").size();
        console.log($(row).find('input:checkbox'));
        $(row).find('input:checkbox').remove();
        //console.log(checkbox);
        //row =  "<div class='multipleRow' style='inline-block'> " . 
        $("<div class='multipleRow' style='inline-block'> " + row + ' </div>').appendTo(container);
        i++;
        return false;
    });

    $(document).on('click', '.removeBtn', function () {
        var i = 0;
        var container = $(this).parents('div.custaGroupContainer');
        i = $(container).children("div").size();
        if (i > 1) {
            $(this).parents('div.multipleRow').remove();
            i--;
        } else {
            // musime zacit od zvrchu
            var row = $(this).parents('div.multipleRow');
            var option = $(row).children("select");
            $(option).prop('selectedIndex', 0);
        }
        return false;
    });
}
);


function deleteRow(element, url) {
    var row = $(element).closest("tr").children('td').find('input');
    var data = {};
    $(row).each(function () {
        var value = $(this).val();
        var name = $(this).attr("name");
        data[name] = value;
    });
    var id = data['id'];
    var surovina = data['id_surovina'];
    
    var url_finalni = url + "/odstran/" + id;
    if ( surovina ){
        url_finalni += '/surovina/' + surovina;
    }
    console.log(data);
    console.log(url);
    console.log(url_finalni);
    //url = 'potraviny';
    $.ajax({
        type: 'GET',
        url: url_finalni,
        data: data,
        contentType: "application/json; charset=utf-8",
        success: function (result) {
            var vysl = JSON.parse(result);
            console.log(vysl);
            if (vysl.success) {
                if (url == 'objednavky') {
                    var value = parseInt($('#cena_celkem').text(),10);
                    console.log(value);
                    value = value + vysl.ztracena_cena;
                    $('#cena_celkem').text(value);
                }
                $(element).closest("tr").hide();
                console.log($(element).closest("tr"));


            }
        }});
}
;

$(function () {
    $('[data-toggle="tooltip"]').tooltip()
})
