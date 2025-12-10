async function loadModal() {
  const response = await fetch('roboticsModal.html');
  const modalHtml = await response.text();
  document.getElementById('roboticsModalContainer').innerHTML = modalHtml;

  const roboticsModal = document.getElementById('roboticsModal');

  roboticsModal.addEventListener('show.bs.modal', () => {

    // Reset scroll position every time modal opens
    const modalContent = roboticsModal.querySelector('.modal-body');
    if (modalContent) modalContent.scrollTop = 0;

    // Load files
    fetch('Files/Wearable.ino')
      .then(r => r.text())
      .then(t => document.getElementById('wifiSender').textContent = t);

    fetch('Files/RobotArm.ino')
      .then(r => r.text())
      .then(t => document.getElementById('wifiReceiver').textContent = t);
  });
}

loadModal();

