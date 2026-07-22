import { addCard } from "../api";
import CardForm from "./CardForm";

const EMPTY_FORM = { title: "", balance: "", expiryDate: "", companies: "" };

export default function AddCard({ onAdd }) {
  async function handleSubmit(card) {
    await addCard(card);
    onAdd();
  }

  return (
    <CardForm
      heading="Add Card"
      initialValues={EMPTY_FORM}
      requireFields
      submitLabel="Add Card"
      onSubmit={handleSubmit}
    />
  );
}
