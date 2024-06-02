document.addEventListener('DOMContentLoaded', function() {
    // find first element in the document with the class menu-toggle
    var toggleButton = document.querySelector('.menu-toggle');

    // find first element in the document with the class nav__content
    var navContent = document.querySelector('.nav-content');

    // when button is clicked
    toggleButton.addEventListener('click', function() {
        // toggle active class to show or hide the navigation content
        navContent.classList.toggle('active');
    });
});
