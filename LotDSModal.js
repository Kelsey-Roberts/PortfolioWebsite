async function loadLotDSModal() {
  // Load the modal HTML into the page
  const response = await fetch('LotDSModal.html');
  const modalHtml = await response.text();
  document.getElementById('LotDSModalContainer').innerHTML += modalHtml;

  const lodsModal = document.getElementById('lodsModal');

  // Autoplay the video when the modal opens
  lodsModal.addEventListener('shown.bs.modal', () => {
    const video = lodsModal.querySelector('video');
    if (video) video.play();
  });

  // Pause the video when the modal closes
  lodsModal.addEventListener('hidden.bs.modal', () => {
    const video = lodsModal.querySelector('video');
    if (video) video.pause();
  });
}

// Call the loader
loadLotDSModal();