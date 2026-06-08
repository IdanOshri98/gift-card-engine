const BASE_URL = "http://localhost:8080/api";

export async function getCards() {
  const res = await fetch(`${BASE_URL}/cards`);
  return res.json();
}

export async function addCard(card) {
  const res = await fetch(`${BASE_URL}/cards`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(card),
  });
  return res.json();
}

export async function updateCard(id, data) {
  const res = await fetch(`${BASE_URL}/cards/${id}`, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(data),
  });
  return res.json();
}

export async function deleteCard(id) {
  const res = await fetch(`${BASE_URL}/cards/${id}`, { method: "DELETE" });
  return res.json();
}

export async function getRiskCards() {
  const res = await fetch(`${BASE_URL}/cards/risk`);
  return res.json();
}

export async function getPlan(by = "expiry") {
  const res = await fetch(`${BASE_URL}/cards/plan?by=${by}`);
  return res.json();
}
