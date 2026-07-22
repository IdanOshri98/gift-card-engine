import { updateCard } from "../api";
import CardForm from "./CardForm";

export default function EditCard({ card, onSave, onCancel }) {
  async function handleSubmit(data) {
    await updateCard(card.id, data);
    onSave();
  }

  return (
    <CardForm
      heading={`Edit Card #${card.id}`}
      initialValues={{
        title: card.title,
        balance: card.balance,
        expiryDate: card.expiryDate,
        companies: card.companies.join(", "),
      }}
      submitLabel="Save"
      onSubmit={handleSubmit}
      onCancel={onCancel}
    />
  );
}
