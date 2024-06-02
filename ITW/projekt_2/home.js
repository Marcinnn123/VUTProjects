document.addEventListener('DOMContentLoaded', function() {
    const hello = document.querySelector('#home .hello');
  
    hello.addEventListener('mouseenter', function() {
      hello.style.color = getRandomColor(); // change text color on hover
      hello.style.cursor = 'default'; // change cursor to default
    });
  
    hello.addEventListener('mouseleave', function() {
      hello.style.color = ''; // reset text color on mouse leave
      hello.style.cursor = 'auto'; // reset cursor to auto
    });
  
    // function to generate a random color
    function getRandomColor() {
      const letters = '0123456789ABCDEF';
      let color = '#';
      for (let i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
      }
      return color;
    }
  });
  